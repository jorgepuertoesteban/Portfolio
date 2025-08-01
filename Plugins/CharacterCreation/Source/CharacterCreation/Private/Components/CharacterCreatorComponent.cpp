// Created by Jorge Puerto. All rights reserved.


#include "Components/CharacterCreatorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CharacterCreator.h"
#include "CharacterCreatorGroom.h"
#include "CharacterCreatorOutfit.h"
#include "GroomBindingAsset.h"
#include "GroomComponent.h"
#include "GroomAsset.h"
#include "Subsystems/CharacterCreationSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/World.h"
#include "CharacterCreationTypes.h"
#include "Net/UnrealNetwork.h"
#include "Interfaces/CharacterCreationDAO.h"
#include "CharacterCreatorOutfitSlot.h"
#include "CharacterCreatorAttribute.h"
#include "CharacterCreatorMatAttribute.h"
#include "CharacterCreatorVectorMatAttribute.h"
#include "Logs/CharacterCreationLog.h"
#include "CharacterCreatorMaterialVariant.h"
#include "CharacterCreatorModel.h"
#include "CharacterCreation.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "CharacterCreatorPoseAttribute.h"
#include "SkeletalMeshMerge.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "CharacterCreatorOutfitMorphAttribute.h"
#include "CharacterCreatorCondition.h"
#include "CharacterCreatorTextureMatAttribute.h"

FName UCharacterCreatorComponent::DynamicCharacterCreatorComponentTag = TEXT("DynamicCharacterCreatorComponent");


UCharacterCreatorComponent::UCharacterCreatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterCreatorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterCreatorComponent, CharacterCreator);
}

void UCharacterCreatorComponent::SetRootSkeletalMesh(USkeletalMeshComponent* NewRootSkComp)
{
	RootSkeletalMesh = NewRootSkComp;
}

UCharacterCreatorComponent* UCharacterCreatorComponent::GetCharacterCreatorComponent(const AActor* ActorOwner)
{
	return ActorOwner ? Cast<UCharacterCreatorComponent>(ActorOwner->GetComponentByClass(UCharacterCreatorComponent::StaticClass())) : nullptr;
}

void UCharacterCreatorComponent::OnRep_CharacterCreator()
{
	SetCharacterCreator(CharacterCreator);
}

void UCharacterCreatorComponent::SetCharacterCreator(UCharacterCreator* NewCharacterCreator, bool bReload /*= false*/)
{
	CharacterCreator = NewCharacterCreator;
	if (bReload)
	{
		ReloadCurrentCharacterCreatorWithAsyncLoad();
	}
}

UGroomComponent* UCharacterCreatorComponent::FindOrCreateGroomComponent(const UCharacterCreatorGroom* NewGroom, const FName& SlotName, bool& bFiltered)
{
	if (!NewGroom || !NewGroom->Slot)
	{
		return nullptr;
	}

	UGroomComponent* GroomComp = SlotGroomMap.FindRef(NewGroom->Slot);

	if (!GroomComp)
	{
		if (NewGroom->Slot->bSpawnsComponent)
		{
			GroomComp = NewObject<UGroomComponent>(GetOwner(), SlotName, EObjectFlags::RF_Transient);
			GroomComp->ComponentTags.Add(DynamicCharacterCreatorComponentTag);
			GetOwner()->AddInstanceComponent(GroomComp);
			GroomComp->SetWorldTransform(FTransform::Identity);
			//GroomComp->AttachToComponent(RootSkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform);
			GroomComp->OnComponentCreated();
			GroomComp->RegisterComponent();
		}
		else 
		{
			if (SlotsToFilterOff.Contains(SlotName))
			{
				bFiltered = true;
				return nullptr;
			}
			for (UActorComponent* OwnerComp : GetOwner()->GetComponents())
			{
				if (UGroomComponent* OwnerGroomComp = Cast<UGroomComponent>(OwnerComp))
				{
					if (FCharacterCreationModule::CharacterCreationDebugVar > 0)
					{
						UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorComponent::SetGroom: tag:[%s] Class: [%s] UGroomComponent to Check [%s]"), *NewGroom->Slot->NonSpawnedComponentTag.ToString(), *GetOwner()->GetName(), *OwnerComp->GetName());
					}

					if (OwnerComp->ComponentHasTag(NewGroom->Slot->NonSpawnedComponentTag))
					{
						if (FCharacterCreationModule::CharacterCreationDebugVar > 0)
						{
							UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorComponent::SetGroom: tag:[%s] Class: [%s] Found "), *NewGroom->Slot->NonSpawnedComponentTag.ToString(), *GetOwner()->GetName());
						}

						GroomComp = OwnerGroomComp;
						break;
					}
				}
			}
			if (!GroomComp)
			{
				if (HasBegunPlay())
				{
					// We only log when the Component has begun play. This is because it might be called before the Blueprint initialization/constructor and Blueprint Created Components aren't accessible yet
					UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreatorComponent::SetGroom: NewGroomSlot set bSpawnsComponent = false but the tag:[%s] is not present in any SkeletalMeshComponent in the Class [%s]"), *NewGroom->Slot->NonSpawnedComponentTag.ToString(), *GetOwner()->GetName());
				}
				return nullptr;
			}
		}
	}

	return GroomComp;
}

void UCharacterCreatorComponent::SetOutfit(const UCharacterCreatorOutfit* NewOutfit, FName SlotName, const int32 MaterialVariantsIndex /*= 0*/)
{
	if (!NewOutfit)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreatorComponent::SetOutfit: NewOufit is not valid"));
		return;
	}

	if (!RootSkeletalMesh)
	{
		//If we can't find a RootSkeletalMesh 
		UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreatorComponent::SetOutfit(): Imposible to find a RootSkeletalMesh for the CharacterCreatorComponent"));
		return;
	}

	USkeletalMeshComponent* SkComp = SlotSKMeshMap.FindRef(NewOutfit->Slot);
	if (!SkComp)
	{
		if (NewOutfit->Slot->bSpawnsComponent)
		{
			USkeletalMeshComponent* CreatedSkComp = NewObject<USkeletalMeshComponent>(GetOwner(), SkeletalMeshClass, SlotName, EObjectFlags::RF_Transient);
			SkComp = CreatedSkComp;
			SkComp->ComponentTags.Add(DynamicCharacterCreatorComponentTag);
			GetOwner()->AddInstanceComponent(SkComp);
			SkComp->bUseAttachParentBound = true;
			SkComp->SetWorldTransform(FTransform::Identity);
			SkComp->AttachToComponent(RootSkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform);
			SkComp->OnComponentCreated();
			SkComp->RegisterComponent();

			CreatedSkComp->SetComponentTickEnabled(false);
			CreatedSkComp->bEnableUpdateRateOptimizations = true;
			SkComp->bReceivesDecals = false;

			for (const FName& Tag : NewOutfit->Slot->Tags)
			{
				SkComp->ComponentTags.Add(Tag);
			}
			SkComp->ComponentTags.Add(NewOutfit->Slot->NonSpawnedComponentTag); //Maybe we shouldn't include this one?
			if (NewOutfit->Slot->bSetLeaderPoseFromRoot && !NewOutfit->Slot->bIsRoot)
			{
				SkComp->SetLeaderPoseComponent(RootSkeletalMesh);
			}
		}
		else
		{
			for (UActorComponent* OwnerComp : GetOwner()->GetComponents())
			{
				if (USkeletalMeshComponent* OwnerSkComp = Cast<USkeletalMeshComponent>(OwnerComp))
				{
					if (FCharacterCreationModule::CharacterCreationDebugVar > 0)
					{
						UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorComponent::SetOutfit: tag:[%s] Class: [%s] SkeletalMeshComponent to Check [%s]"), *NewOutfit->Slot->NonSpawnedComponentTag.ToString(), *GetOwner()->GetName(), *OwnerComp->GetName());
					}

					if (OwnerComp->ComponentHasTag(NewOutfit->Slot->NonSpawnedComponentTag))
					{
						if (FCharacterCreationModule::CharacterCreationDebugVar > 0)
						{
							UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorComponent::SetOutfit: tag:[%s] Class: [%s] Found "), *NewOutfit->Slot->NonSpawnedComponentTag.ToString(), *GetOwner()->GetName());
						}

						if (NewOutfit->Slot->bSetLeaderPoseFromRoot && !NewOutfit->Slot->bIsRoot)
						{
							OwnerSkComp->SetLeaderPoseComponent(RootSkeletalMesh);
						}

						SkComp = OwnerSkComp;
						break;
					}
				}
			}
			if (!SkComp)
			{
				if (HasBegunPlay())
				{
					//We only log when the Component has begun play. This is because it might be called before the Blueprint intialization/constructor and Blueprint Created Components aren't accessible yet
					UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreatorComponent::SetOutfit: OufitSlot set bSpawnsComponent = false but the tag:[%s], is not present in any SkeletalMeshComponent in the Class [%s]"), *NewOutfit->Slot->NonSpawnedComponentTag.ToString(), *GetOwner()->GetName());
				}
				return;
			}
		}
	}

	if (FaceSlot && NewOutfit->Slot == FaceSlot)
	{
		FaceSkeletalMesh = SkComp;
	}

	SlotSKMeshMap.Add(NewOutfit->Slot, SkComp);
	SlotMeshMap.Add(NewOutfit->Slot, SkComp);

	const uint8 Index = static_cast<uint8>(CharacterCreator->BodyType);
	if (NewOutfit->Meshes.IsValidIndex(Index))
	{
		const TSoftObjectPtr<USkeletalMesh> SoftSkeletal = NewOutfit->Meshes[Index];
		const FSoftObjectPath SoftObjectPath = SoftSkeletal.ToSoftObjectPath();

		TWeakObjectPtr<UCharacterCreatorComponent> ThisWeak = this;
		TWeakObjectPtr<USkeletalMeshComponent> SkCompWeak = SkComp;
		TWeakObjectPtr<const UCharacterCreatorOutfit> NewOutfitWeak = NewOutfit;

		UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftObjectPath, [this, ThisWeak, SkCompWeak, NewOutfitWeak, SoftSkeletal, MaterialVariantsIndex]()
		{
			if (!ThisWeak.IsValid() || !SkCompWeak.IsValid() || !NewOutfitWeak.IsValid() || !SoftSkeletal.Get())
			{
				return;
			}

			TMap<const UCharacterCreatorPoseAttribute*, float> AffectedPoseAttributes;

			for (const FCCPoseAttributeValue& PoseAttributeValue : CharacterCreator->PoseAttributeValues)
			{
				if (PoseAttributeValue.Attribute == nullptr)
				{
					continue;
				}
				if (PoseAttributeValue.Value != 0 && PoseAttributeValue.Attribute->Slot == NewOutfitWeak->Slot)
				{
					AffectedPoseAttributes.Add(PoseAttributeValue.Attribute, PoseAttributeValue.Value);
				}
			}

			if (AffectedPoseAttributes.IsEmpty())
			{
				DuplicatedSkeletalMeshes.Remove(NewOutfitWeak->Slot);
				SkCompWeak->SetSkeletalMesh(SoftSkeletal.Get());
			}
			else
			{
				if (!DuplicatedSkeletalMeshes.Find(NewOutfitWeak->Slot) || DuplicatedSkeletalMeshes[NewOutfitWeak->Slot].Duplication == nullptr || DuplicatedSkeletalMeshes[NewOutfitWeak->Slot].Original != SoftSkeletal.Get())
				{
					USkeletalMesh* Original = SoftSkeletal.Get();

					TArray<USkeletalMesh*> MeshesToMergeCopy;
					MeshesToMergeCopy.Add(Original);

					TArray<FSkelMeshMergeSectionMapping> SectionMappings;

					USkeletalMesh* Duplication = NewObject<USkeletalMesh>();

					FSkeletalMeshMerge Merger(Duplication, MeshesToMergeCopy, SectionMappings, 0);
					Merger.DoMerge();

					Duplication->SetSkeleton(Original->GetSkeleton());
					Duplication->SetPhysicsAsset(Original->GetPhysicsAsset());

					SkCompWeak->SetSkeletalMesh(Duplication);

					DuplicatedSkeletalMeshes.Add(NewOutfitWeak->Slot, FDuplicatedSkeletalMesh(Original, Duplication));
				}

				FReferenceSkeleton* ModifyRefSkeleton = &DuplicatedSkeletalMeshes[NewOutfitWeak->Slot].Duplication->GetRefSkeleton();
				const FReferenceSkeleton* ReferenceSkeleton = &DuplicatedSkeletalMeshes[NewOutfitWeak->Slot].Original->GetRefSkeleton();

				FReferenceSkeletonModifier SkeletonModifier(*ModifyRefSkeleton, DuplicatedSkeletalMeshes[NewOutfitWeak->Slot].Duplication->GetSkeleton());

				TMap<FName, FTransform> PoseOffsets;

				for (auto& AffectedPoseAttributeElem : AffectedPoseAttributes)
				{
					if (AffectedPoseAttributeElem.Value < 0.f)
					{
						for (auto& PoseOffsetsElem : AffectedPoseAttributeElem.Key->MinPoseOffsets)
						{
							FTransform AttributeValue = FTransform(FRotator::ZeroRotator, PoseOffsetsElem.Value.GetLocation() * FMath::Abs(AffectedPoseAttributeElem.Value) * AffectedPoseAttributeElem.Key->Power, PoseOffsetsElem.Value.GetScale3D() * FMath::Abs(AffectedPoseAttributeElem.Value));
							PoseOffsets.Add(PoseOffsetsElem.Key, PoseOffsets.Find(PoseOffsetsElem.Key) ? PoseOffsets[PoseOffsetsElem.Key] + AttributeValue : AttributeValue);
						}
					}
					else
					{
						for (auto& PoseOffsetsElem : AffectedPoseAttributeElem.Key->MaxPoseOffsets)
						{
							FTransform AttributeValue = FTransform(FRotator::ZeroRotator, PoseOffsetsElem.Value.GetLocation() * FMath::Abs(AffectedPoseAttributeElem.Value) * AffectedPoseAttributeElem.Key->Power, PoseOffsetsElem.Value.GetScale3D() * FMath::Abs(AffectedPoseAttributeElem.Value));
							PoseOffsets.Add(PoseOffsetsElem.Key, PoseOffsets.Find(PoseOffsetsElem.Key) ? PoseOffsets[PoseOffsetsElem.Key] + AttributeValue : AttributeValue);
						}
					}
				}

				for (auto& PoseOffset : PoseOffsets)
				{
					int32 BoneIndex = ModifyRefSkeleton->FindBoneIndex(PoseOffset.Key);

					if (BoneIndex == -1)
					{
						continue;
					}

					FTransform AttributeValue = ReferenceSkeleton->GetRefBonePose()[BoneIndex] + PoseOffset.Value;
					AttributeValue.NormalizeRotation();
					SkeletonModifier.UpdateRefPoseTransform(BoneIndex, AttributeValue);
				}
			}

			SkCompWeak->EmptyOverrideMaterials();

			if (NewOutfitWeak->MaterialVariants.IsValidIndex(MaterialVariantsIndex))
			{
				const TArray<TSoftObjectPtr<UMaterialInterface>>& Variants = NewOutfitWeak->MaterialVariants[MaterialVariantsIndex].Materials;

				for (int32 i = 0; i < Variants.Num(); i++)
				{
					SkCompWeak->SetMaterial(i, Variants[i].LoadSynchronous());
				}
			}

			int32 const NumMaterials = SkCompWeak->GetNumMaterials();
			for (int32 i = 0; i < NumMaterials; i++)
			{
				UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(SkCompWeak->GetMaterial(i));
				if (!MID)
				{
					MID = SkCompWeak->CreateDynamicMaterialInstance(i);
				}
				for (const FCCMaterialAttributeValue& MaterialAttributeValue : CharacterCreator->MaterialAttributeValues)
				{
					if (MaterialAttributeValue.MaterialAttribute)
					{
						MaterialAttributeValue.MaterialAttribute->SetAttributeOnMaterialInstanceDynamic(MID, MaterialAttributeValue.Value);
					}
				}
				for (const FCCVectorMaterialAttributeValue& MaterialAttributeValue : CharacterCreator->VectorMaterialAttributeValues)
				{
					if (MaterialAttributeValue.AffectedSlots.Contains(NewOutfitWeak->Slot) && MaterialAttributeValue.MaterialAttribute)
					{
						MaterialAttributeValue.MaterialAttribute->SetAttributeOnMaterialInstanceDynamic(MID, MaterialAttributeValue.Value);
					}
				}
			}

			for (const FCharacterCreatorOutfitMask& Mask : NewOutfitWeak->Masks)
			{
				if (USkeletalMeshComponent* MaskedSkComp = Mask.bIsRoot ? RootSkeletalMesh : SlotSKMeshMap[Mask.OutfitSlot])
				{
					UMaterialInstanceDynamic* MaterialToMask = Cast<UMaterialInstanceDynamic>(MaskedSkComp->GetMaterial(Mask.MaterialIndex));
					if (!MaterialToMask)
					{
						MaskedSkComp->CreateDynamicMaterialInstance(Mask.MaterialIndex); //It will never be executed
					}
					if (MaterialToMask)
					{
						MaterialToMask->SetTextureParameterValue(Mask.ParameterName, Mask.TextureMask);
					}
				}
			}

			//Set morphs in this Outfit
			for (const FCharacterCreatorOutfitMorphAttribute& MorphAttribute : NewOutfitWeak->Morphs)
			{
				if (USkeletalMeshComponent* MorphedSkComp = SlotSKMeshMap[MorphAttribute.OutfitSlot])
				{
					MorphedSkComp->SetMorphTarget(MorphAttribute.MorphName, MorphAttribute.Value);
				}
			}


			for (const FCCMOUnderConditions& ModifierWithConditions : NewOutfitWeak.Get()->ModifiersWithConditions)
			{
				bool bAllConditionsMet = true;
				for (const UCharacterCreatorCondition* Condition : ModifierWithConditions.Conditions)
				{
					if (!Condition)
					{
						continue;
					}
					if (!Condition->IsConditionMet(NewOutfitWeak.Get(), CharacterCreator))
					{
						bAllConditionsMet = false;
					}
				}
				if (!ModifierWithConditions.Modifier)
				{
					continue;
				}
				if (!bAllConditionsMet)
				{
					continue;
				}
				ModifierWithConditions.Modifier->PerformModification(this, SkCompWeak.Get(), NewOutfitWeak.Get(), CharacterCreator);
			}

			//Set morphs pointing to this slot in the rest of the outfits
			for (const FCCSlotAndOutfit& SlotAndOutfit : CharacterCreator->SlotAndOutfitArray)
			{
				if (SlotAndOutfit.Slot == nullptr || SlotAndOutfit.Outfit == nullptr)
				{
					continue;
				}
				for (const FCharacterCreatorOutfitMorphAttribute& MorphAttribute : SlotAndOutfit.Outfit->Morphs)
				{
					if (MorphAttribute.OutfitSlot != NewOutfitWeak->Slot)
					{
						continue;
					}
					SkCompWeak->SetMorphTarget(MorphAttribute.MorphName, MorphAttribute.Value);
				}
			}

			//if (RootSkeletalMesh)
			//{
				//We first reset all the modifiers
				//if (CharacterCreator->Model)
				//{
				//	for (const FCharacterCreatorOutfitModifier& Modifier : CharacterCreator->Model->DefaultModifiers)
				//	{
				//		RootSkeletalMesh->SetMorphTarget(Modifier.MorphTargetName, Modifier.Value);
				//	}
				//}
				//Then we apply the Outfit Modifiers
				//for (const FCharacterCreatorOutfitModifier& Modifier : NewOutfitWeak->Modifiers)
				//{
				//	RootSkeletalMesh->SetMorphTarget(Modifier.MorphTargetName, Modifier.Value);
				//}
			//}

			if (NewOutfitWeak->Slot->bRefreshGrooms)
			{
				for (const FCCSlotAndGroom& SlotAndGroom : CharacterCreator->SlotAndGroomArray)
				{
					if (SlotAndGroom.Slot == nullptr)
					{
						continue;
					}
					SetGroom(SlotAndGroom.Groom, FName(SlotAndGroom.Slot->Name));
				}
			}
			OnOutfitChanged.Broadcast(SkCompWeak.Get());
		});
	}
}

bool UCharacterCreatorComponent::IsUsingOutfit(const UCharacterCreatorOutfit* Outfit, const int32 MaterialVariantIndex/*= -1*/) const
{
	if (GetCharacterCreator() == nullptr)
	{
		return false;
	}

	return GetCharacterCreator()->IsUsingOutfit(Outfit, MaterialVariantIndex);
}

void UCharacterCreatorComponent::SetGroom(const UCharacterCreatorGroom* NewGroom, const FName& SlotName)
{
	if (!NewGroom)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreatorComponent::SetGroom - NewGroom is not valid for Slot [%s]"), *SlotName.ToString());
		return;
	}
	bool bFiltered = false;
	UGroomComponent* GroomComp = FindOrCreateGroomComponent(NewGroom, SlotName, bFiltered);
	if (!GroomComp)
	{
		if (!bFiltered)
		{
			UE_LOG(
				CharacterCreationLog,
				Error,
				TEXT("UCharacterCreatorComponent::SetGroom - GroomComp is not valid. Groom: [%s] Actor: [%s],  Slot [%s]"),
				*NewGroom->GetName(),
				*GetOwner()->GetName(),
				*SlotName.ToString()
			);
		}
		return;
	}
	GroomComp->SetGroomAsset(nullptr);
	GroomComp->SetBindingAsset(nullptr);

	if (NewGroom->GroomAsset.IsNull())
	{
		return;
	}

	// Set up groom binding
	const USkeletalMeshComponent* GroomAttachmentSKMeshComponent = Cast<USkeletalMeshComponent>(SlotMeshMap.FindRef(NewGroom->SlotToAttach));
	if (!GroomAttachmentSKMeshComponent)
	{
		UE_LOG(
			CharacterCreationLog,
			Log,
			TEXT("UCharacterCreatorComponent::SetGroom - No GroomAttachmentSKMeshComponent found in SlotMeshMap for Slot [%s]"),
			*SlotName.ToString());
		return;
	}

	const USkeletalMesh* SkeletalMesh = DuplicatedSkeletalMeshes.Find(NewGroom->SlotToAttach) != nullptr ? DuplicatedSkeletalMeshes[NewGroom->SlotToAttach].Original : GroomAttachmentSKMeshComponent->GetSkeletalMeshAsset();
	if (!SkeletalMesh)
	{
		UE_LOG(
			CharacterCreationLog,
			Log,
			TEXT("UCharacterCreatorComponent::SetGroom - No SkeletalMesh found in GroomAttachmentSKMeshComponent:  Groom: [%s], Actor: [%s], GroomAttachmentSKMeshComponent: [%s], Slot [%s]"),
			*NewGroom->GetName(),
			*GetOwner()->GetName(),
			*GroomAttachmentSKMeshComponent->GetName(),
			*SlotName.ToString()
		);
		return;
	}
	if (!NewGroom->BindingsMap.Contains(SkeletalMesh))
	{
		UE_LOG(
			CharacterCreationLog,
			Log,
			TEXT("UCharacterCreatorComponent::SetGroom - Groom has no associated GroomBindingAsset for USkeletalMesh. Groom: [%s], Actor: [%s], GroomAttachmentSKMeshComponent: [%s], SkeletalMesh: [%s] Slot [%s]"),
			*NewGroom->GetName(),
			*GetOwner()->GetName(),
			*GroomAttachmentSKMeshComponent->GetName(),
			*SkeletalMesh->GetName(),
			*SlotName.ToString()
		);
		return;
	}


	const auto SoftGroomBinding = NewGroom->BindingsMap.Find(SkeletalMesh);
	if (!SoftGroomBinding)
	{
		UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorComponent::SetGroom - No SoftGroomBinding found"));
		return;
	}

	if (ItemStreamingHandle.IsValid() && ItemStreamingHandle->IsLoadingInProgress())
	{
		ItemStreamingHandle->CancelHandle();
	}

	TArray<FSoftObjectPath> ItemsToStream;
	ItemsToStream.Add(NewGroom->GroomAsset.ToSoftObjectPath());
	ItemsToStream.Add(SoftGroomBinding->ToSoftObjectPath());

	TWeakObjectPtr<UGroomComponent> GroomCompWeak = GroomComp;
	TWeakObjectPtr<const UCharacterCreatorGroom> NewGroomWeak = NewGroom;

	ItemStreamingHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(ItemsToStream, [this, GroomCompWeak, SoftGroomBinding, NewGroomWeak, SlotName]()
	{
		if (!GroomCompWeak.IsValid() || !NewGroomWeak.IsValid() || !NewGroomWeak->GroomAsset.IsValid())
		{
			UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorComponent::SetGroom#RequestAsyncLoad - GroomCompWeak or NewGroomWeakPtr is no longer Valid"));
			return;
		}
		USkeletalMeshComponent* GroomAttachmentSKMeshComponent = Cast<USkeletalMeshComponent>(SlotMeshMap.FindRef(NewGroomWeak->SlotToAttach));
		if (!GroomAttachmentSKMeshComponent)
		{
			UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorComponent::SetGroom#RequestAsyncLoad - No GroomAttachmentSKMeshComponent"));
			return;
		}
		const USkeletalMesh* SkeletalMesh = DuplicatedSkeletalMeshes.Find(NewGroomWeak->SlotToAttach) != nullptr ? DuplicatedSkeletalMeshes[NewGroomWeak->SlotToAttach].Original : GroomAttachmentSKMeshComponent->GetSkeletalMeshAsset();
		if (!SkeletalMesh)
		{
			UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorComponent::SetGroom#RequestAsyncLoad - No SkeletalMesh found in GroomAttachmentSKMeshComponent"));
			return;
		}
		TArray<FHairGroupsMaterial> GroomMaterials = NewGroomWeak->GroomAsset->GetHairGroupsMaterials();
		for (int32 Index = 0; Index < GroomMaterials.Num(); Index++)
		{
			GroomCompWeak->SetMaterial(Index, GroomMaterials[Index].Material);
		}

		GroomCompWeak->SetBindingAsset(SoftGroomBinding->Get());
		GroomCompWeak->SetGroomAsset(NewGroomWeak->GroomAsset.Get());
		GroomCompWeak->AttachmentName = NewGroomWeak->Slot->AttachementName;
		GroomCompWeak->AttachToComponent(GroomAttachmentSKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
		int32 const NumMaterials = GroomCompWeak->GetNumMaterials();
		for (int32 i = 0; i < NumMaterials; i++)
		{
			UMaterialInstanceDynamic* MID = GroomCompWeak->CreateDynamicMaterialInstance(i);
			for (const FCCMaterialAttributeValue& MaterialAttributeValue : CharacterCreator->MaterialAttributeValues)
			{
				if (MaterialAttributeValue.MaterialAttribute)
				{
					MaterialAttributeValue.MaterialAttribute->SetAttributeOnMaterialInstanceDynamic(MID, MaterialAttributeValue.Value);
				}
			}
			for (const FCCVectorMaterialAttributeValue& MaterialAttributeValue : CharacterCreator->VectorMaterialAttributeValues)
			{
				if (MaterialAttributeValue.AffectedSlots.Contains(NewGroomWeak->Slot) && MaterialAttributeValue.MaterialAttribute)
				{
					MaterialAttributeValue.MaterialAttribute->SetAttributeOnMaterialInstanceDynamic(MID, MaterialAttributeValue.Value);
				}
			}
		}


		SlotGroomMap.Add(NewGroomWeak.Get()->Slot, GroomCompWeak.Get());
		SlotMeshMap.Add(NewGroomWeak.Get()->Slot, GroomCompWeak.Get());

		for (const FCCTextureMaterialAttributeValue& TextureMaterialAttributeValue : NewGroomWeak->TextureMaterialAttributeValues)
		{
			if (!TextureMaterialAttributeValue.TextureMaterialAttribute && !TextureMaterialAttributeValue.Value)
			{
				continue;
			}
			for (const UCharacterCreatorOutfitSlot* CharacterCreatorOutfitSlot : TextureMaterialAttributeValue.AffectedSlots)
			{
				USkeletalMeshComponent* SKMeshComponent = Cast<USkeletalMeshComponent>(SlotMeshMap.FindRef(CharacterCreatorOutfitSlot));
				if (!SKMeshComponent)
				{
					continue;
				}
				int32 const NumTextureMaterials = SKMeshComponent->GetNumMaterials();
				for (int32 i = 0; i < NumTextureMaterials; i++)
				{
					UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(SKMeshComponent->GetMaterial(i));
					if (!MID)
					{
						MID = SKMeshComponent->CreateDynamicMaterialInstance(i);
					}
					TextureMaterialAttributeValue.TextureMaterialAttribute->SetAttributeOnMaterialInstanceDynamic(MID, TextureMaterialAttributeValue.Value);
				}
			}
		}
		OnGroomChanged.Broadcast(GroomCompWeak.Get());
	});
}

void UCharacterCreatorComponent::SetMaterialAttribute(const UCharacterCreatorMatAttribute* MaterialAttribute, float Value)
{
	if (MaterialAttribute)
	{
		for (const TTuple<const UCharacterCreatorOutfitSlot*, USkeletalMeshComponent*> SlotMesh : SlotSKMeshMap)
		{
			int32 const NumMaterials = SlotMesh.Value->GetNumMaterials();
			for (int32 i = 0; i < NumMaterials; i++)
			{
				UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(SlotMesh.Value->GetMaterial(i));
				MaterialAttribute->SetAttributeOnMaterialInstanceDynamic(Material, Value);
			}
		}
		for (const TTuple<const UCharacterCreatorOutfitSlot*, UGroomComponent*> SlotGroom : SlotGroomMap)
		{
			int32 const NumMaterials = SlotGroom.Value->GetNumMaterials();
			for (int32 i = 0; i < NumMaterials; i++)
			{
				UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(SlotGroom.Value->GetMaterial(i));
				MaterialAttribute->SetAttributeOnMaterialInstanceDynamic(Material, Value);
			}
		}
	}
}

void UCharacterCreatorComponent::SetVectorMaterialAttribute(const UCharacterCreatorVectorMatAttribute* MaterialAttribute, FLinearColor Value)
{
	ReloadCurrentCharacterCreatorWithAsyncLoad();
}

void UCharacterCreatorComponent::SetPoseAttribute(const UCharacterCreatorPoseAttribute* PoseAttribute, float Value)
{
	ReloadCurrentCharacterCreatorWithAsyncLoad();
}

bool UCharacterCreatorComponent::LoadCharacterCreatorFromDatabase()
{
	const UCharacterCreationSubsystem* CCSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UCharacterCreationSubsystem>();
	if (CCSubsystem && CCSubsystem->GetDao())
	{
		//CCSubsystem->GetDao()->LoadCharacterCreator(DatabaseId, OnLoadToDBDelegate);
		return true;
	}
	return false;
}

bool UCharacterCreatorComponent::SaveCharacterCreatorToDatabase()
{
	const UCharacterCreationSubsystem* CCSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UCharacterCreationSubsystem>();
	if (CCSubsystem && CCSubsystem->GetDao())
	{
		FSaveCharacterCreatorDelegate Delegate;
		CCSubsystem->GetDao()->SaveCharacterCreator(CharacterCreator, Delegate);
		return true;
	}
	return false;
}

void UCharacterCreatorComponent::OnLoadDaoResponse(const FLoadCharacterCreatorResponse& Response)
{
	if (Response.Success)
	{
		SetCharacterCreator(Response.CharacterCreator);
		UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorComponent::OnLoadDaoResponse Succesfully Loaded."))
	}
	else
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreatorComponent::OnLoadDaoResponse error: %s"), *Response.Message)
	}
}

void UCharacterCreatorComponent::OnChangedReceived()
{
	//ReloadCurrentCharacterCreator();

	//if (GetOwner()->GetLocalRole() == ROLE_Authority)
	//{
	//	SaveCharacterCreatorToDatabase();
	//}
}

void UCharacterCreatorComponent::ReloadCurrentCharacterCreator()
{
	if (CharacterCreator != CharacterCreatorLastUsed)
	{
		if (CharacterCreatorLastUsed)
		{
			CharacterCreatorLastUsed->OnOutfitChangedDelegate.RemoveDynamic(this, &UCharacterCreatorComponent::OnOutfitChangedReceived);
			CharacterCreatorLastUsed->OnGroomChangedDelegate.RemoveDynamic(this, &UCharacterCreatorComponent::OnGroomChangedReceived);

			CharacterCreatorLastUsed->OnAttributeChangedDelegate.RemoveDynamic(this, &UCharacterCreatorComponent::OnAttributeChangedReceived);
			CharacterCreatorLastUsed->OnMaterialAttributeChangedDelegate.RemoveDynamic(this, &UCharacterCreatorComponent::OnMaterialAttributeChangedReceived);

			CharacterCreatorLastUsed->OnBodyTypeChangedDelegate.RemoveDynamic(this, &UCharacterCreatorComponent::OnBodyTypeChangedReceived);
		}

		if (CharacterCreator)
		{
			CharacterCreator->OnOutfitChangedDelegate.AddDynamic(this, &UCharacterCreatorComponent::OnOutfitChangedReceived);
			CharacterCreator->OnGroomChangedDelegate.AddDynamic(this, &UCharacterCreatorComponent::OnGroomChangedReceived);

			CharacterCreator->OnAttributeChangedDelegate.AddDynamic(this, &UCharacterCreatorComponent::OnAttributeChangedReceived);
			CharacterCreator->OnMaterialAttributeChangedDelegate.AddDynamic(this, &UCharacterCreatorComponent::OnMaterialAttributeChangedReceived);
			CharacterCreator->OnVectorMaterialAttributeChangedDelegate.AddDynamic(this, &UCharacterCreatorComponent::OnVectorMaterialAttributeChangedReceived);
			CharacterCreator->OnPoseAttributeChangedDelegate.AddDynamic(this, &UCharacterCreatorComponent::OnPoseAttributeChangedReceived);

			CharacterCreator->OnBodyTypeChangedDelegate.AddDynamic(this, &UCharacterCreatorComponent::OnBodyTypeChangedReceived);
		}
	}

	//Destroy Meshes and empty SlotSKMeshMap, they aren't valid anymore
	for (const TTuple<const UCharacterCreatorOutfitSlot*, USkeletalMeshComponent*> SlotMesh : SlotSKMeshMap)
	{
		if (SlotMesh.Key && SlotMesh.Value)
		{
			if (SlotMesh.Key->bSpawnsComponent)
			{
				SlotMesh.Value->DestroyComponent(false);
			}
			//else
			//{
			//	SlotMes.Value->SetSkeletalMesh(nullptr);
			//}
		}
	}

	SlotSKMeshMap.Empty();

	//Destroy Grooms and empty SlotGroomMap, they aren't valid anymore
	for (const TTuple<const UCharacterCreatorOutfitSlot*, UGroomComponent*> SlotGroom : SlotGroomMap)
	{
		if (SlotGroom.Key->bSpawnsComponent && SlotGroom.Value)
		{
			SlotGroom.Value->DestroyComponent(false);
		}
	}

	SlotGroomMap.Empty();

	SlotMeshMap.Empty();

	CharacterCreatorLastUsed = CharacterCreator; //This is crucial to keep track of the generation

	if (CharacterCreator)
	{
		if (!RootSkeletalMesh)
		{
			SetDefaultRootSkeletalMesh();
		}

		if (bSearchSkeletalMeshesInActor)
		{
			TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
			GetOwner()->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);
			for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
			{
				SkeletalMeshComponent->SetComponentTickEnabled(false);
				SkeletalMeshComponent->SetVisibility(false, true);
			}
		}
		if (bSearchGroomsInActor)
		{
			TArray<UGroomComponent*> GroomComponents;
			GetOwner()->GetComponents<UGroomComponent>(GroomComponents);
			for (UGroomComponent* GroomComponent : GroomComponents)
			{
				GroomComponent->SetComponentTickEnabled(false);
				GroomComponent->SetVisibility(false, true);
			}
		}

		//I assign all outfits regardless of they are different or not
		for (const FCCSlotAndOutfit& SlotAndOutfit : CharacterCreator->SlotAndOutfitArray)
		{
			if (SlotAndOutfit.Slot == nullptr)			
			{
				continue;
			}
			SetOutfit(SlotAndOutfit.Outfit, FName(SlotAndOutfit.Slot->Name), SlotAndOutfit.MaterialVariantIndex);
		}

		// for (const FCCSlotAndGroom& SlotAndGroom : CharacterCreator->SlotAndGroomArray)
		// {
		// 	SetGroom(SlotAndGroom.Groom, FName(SlotAndGroom.Slot->Name));
		// }
		//
		for (const FCCAttributeValue& AttributeValue : CharacterCreator->AttributeValues)
		{
			SetMorphTarget(AttributeValue.Attribute, AttributeValue.Value);
		}
	}
}

void UCharacterCreatorComponent::ReloadCurrentCharacterCreatorWithAsyncLoad()
{
	if (CharacterCreator != CharacterCreatorLastUsed)
	{
		if (CharacterCreatorLastUsed)
		{
			CharacterCreatorLastUsed->OnOutfitChangedDelegate.RemoveDynamic(this, &UCharacterCreatorComponent::OnOutfitChangedReceived);
			CharacterCreatorLastUsed->OnGroomChangedDelegate.RemoveDynamic(this, &UCharacterCreatorComponent::OnGroomChangedReceived);

			CharacterCreatorLastUsed->OnAttributeChangedDelegate.RemoveDynamic(this, &UCharacterCreatorComponent::OnAttributeChangedReceived);
			CharacterCreatorLastUsed->OnMaterialAttributeChangedDelegate.RemoveDynamic(this, &UCharacterCreatorComponent::OnMaterialAttributeChangedReceived);

			CharacterCreatorLastUsed->OnBodyTypeChangedDelegate.RemoveDynamic(this, &UCharacterCreatorComponent::OnBodyTypeChangedReceived);
		}

		if (CharacterCreator)
		{
			CharacterCreator->OnOutfitChangedDelegate.AddDynamic(this, &UCharacterCreatorComponent::OnOutfitChangedReceived);
			CharacterCreator->OnGroomChangedDelegate.AddDynamic(this, &UCharacterCreatorComponent::OnGroomChangedReceived);

			CharacterCreator->OnAttributeChangedDelegate.AddDynamic(this, &UCharacterCreatorComponent::OnAttributeChangedReceived);
			CharacterCreator->OnMaterialAttributeChangedDelegate.AddDynamic(this, &UCharacterCreatorComponent::OnMaterialAttributeChangedReceived);
			CharacterCreator->OnVectorMaterialAttributeChangedDelegate.AddDynamic(this, &UCharacterCreatorComponent::OnVectorMaterialAttributeChangedReceived);
			CharacterCreator->OnPoseAttributeChangedDelegate.AddDynamic(this, &UCharacterCreatorComponent::OnPoseAttributeChangedReceived);

			CharacterCreator->OnBodyTypeChangedDelegate.AddDynamic(this, &UCharacterCreatorComponent::OnBodyTypeChangedReceived);
		}
	}

	//Destroy Meshes and empty SlotSKMeshMap, they aren't valid anymore
	for (const TTuple<const UCharacterCreatorOutfitSlot*, USkeletalMeshComponent*> SlotMesh : SlotSKMeshMap)
	{
		if (SlotMesh.Key && SlotMesh.Value)
		{
			if (SlotMesh.Key->bSpawnsComponent)
			{
				SlotMesh.Value->DestroyComponent(false);
			}
			//else
			//{
			//	SlotMes.Value->SetSkeletalMesh(nullptr);
			//}
		}
	}

	SlotSKMeshMap.Empty();

	//Destroy Grooms and empty SlotGroomMap, they aren't valid anymore
	for (const TTuple<const UCharacterCreatorOutfitSlot*, UGroomComponent*> SlotGroom : SlotGroomMap)
	{
		if (SlotGroom.Key->bSpawnsComponent && SlotGroom.Value)
		{
			SlotGroom.Value->DestroyComponent(false);
		}
	}

	SlotGroomMap.Empty();
	SlotMeshMap.Empty();

	CharacterCreatorLastUsed = CharacterCreator; //This is crucial to keep track of the generation

	TArray<FSoftObjectPath> AssetsToStream;

	if (CharacterCreator)
	{
		if (!RootSkeletalMesh)
		{
			SetDefaultRootSkeletalMesh();
		}

		if (bSearchSkeletalMeshesInActor)
		{
			TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
			GetOwner()->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);
			for (USkeletalMeshComponent* SkeletalMeshComponent : SkeletalMeshComponents)
			{
				SkeletalMeshComponent->SetComponentTickEnabled(false);
				SkeletalMeshComponent->SetVisibility(false, true);
			}
		}
		if (bSearchGroomsInActor)
		{
			TArray<UGroomComponent*> GroomComponents;
			GetOwner()->GetComponents<UGroomComponent>(GroomComponents);
			for (UGroomComponent* GroomComponent : GroomComponents)
			{
				GroomComponent->SetComponentTickEnabled(false);
				GroomComponent->SetVisibility(false, true);
			}
		}

		//I assign all outfits regardless of they are different or not
		for (const FCCSlotAndOutfit& SlotAndOutfit : CharacterCreator->SlotAndOutfitArray)
		{
			if (SlotAndOutfit.Slot == nullptr)
			{
				continue;
			}
			OutfitLoadAsync(AssetsToStream, SlotAndOutfit.Outfit, FName(SlotAndOutfit.Slot->Name), SlotAndOutfit.MaterialVariantIndex);
		}
	}

	if (ItemStreamingHandle.IsValid() && ItemStreamingHandle->IsLoadingInProgress())
	{
		ItemStreamingHandle->CancelHandle();
	}


	TWeakObjectPtr<ThisClass> WeakThis = this;
	ItemStreamingHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(AssetsToStream, [WeakThis, AssetsToStream]()
	{
		if (ThisClass* StrongThis = WeakThis.Get())
		{
			//I assign all outfits regardless of they are different or not
			for (const FCCSlotAndOutfit& SlotAndOutfit : StrongThis->CharacterCreator->SlotAndOutfitArray)
			{
				if (SlotAndOutfit.Slot == nullptr)
				{
					continue;
				}
				StrongThis->SetOutfitLoadedAsset(SlotAndOutfit.Outfit, FName(SlotAndOutfit.Slot->Name), SlotAndOutfit.MaterialVariantIndex);
			}
		}
	});
}

void UCharacterCreatorComponent::OutfitLoadAsync(TArray<FSoftObjectPath>& AssetsToStream, const UCharacterCreatorOutfit* NewOutfit, FName SlotName, const int32 MaterialVariantsIndex)
{
	if (!NewOutfit)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreatorComponent::SetOutfitLoadAsync: NewOufit is not valid"));
		return;
	}

	if (!RootSkeletalMesh)
	{
		//If we can't find a RootSkeletalMesh 
		UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreatorComponent::SetOutfitLoadAsync(): Imposible to find a RootSkeletalMesh for the CharacterCreatorComponent"));
		return;
	}

	USkeletalMeshComponent* SkComp = SlotSKMeshMap.FindRef(NewOutfit->Slot);
	if (!SkComp)
	{
		if (NewOutfit->Slot->bSpawnsComponent)
		{
			USkeletalMeshComponent* CreatedSkComp = NewObject<USkeletalMeshComponent>(GetOwner(), SkeletalMeshClass, SlotName, EObjectFlags::RF_Transient);
			SkComp = CreatedSkComp;
			SkComp->ComponentTags.Add(DynamicCharacterCreatorComponentTag);
			GetOwner()->AddInstanceComponent(SkComp);
			SkComp->bUseAttachParentBound = true;
			SkComp->SetWorldTransform(FTransform::Identity);
			SkComp->AttachToComponent(RootSkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform);
			SkComp->OnComponentCreated();
			SkComp->RegisterComponent();

			CreatedSkComp->SetComponentTickEnabled(false);
			CreatedSkComp->bEnableUpdateRateOptimizations = true;
			SkComp->bReceivesDecals = false;

			for (const FName& Tag : NewOutfit->Slot->Tags)
			{
				SkComp->ComponentTags.Add(Tag);
			}
			SkComp->ComponentTags.Add(NewOutfit->Slot->NonSpawnedComponentTag); //Maybe we shouldn't include this one?
			if (NewOutfit->Slot->bSetLeaderPoseFromRoot && !NewOutfit->Slot->bIsRoot)
			{
				SkComp->SetLeaderPoseComponent(RootSkeletalMesh);
			}
		}
		else
		{
			for (UActorComponent* OwnerComp : GetOwner()->GetComponents())
			{
				if (USkeletalMeshComponent* OwnerSkComp = Cast<USkeletalMeshComponent>(OwnerComp))
				{
					if (FCharacterCreationModule::CharacterCreationDebugVar > 0)
					{
						UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorComponent::SetOutfitLoadAsync: tag:[%s] Class: [%s] SkeletalMeshComponent to Check [%s]"), *NewOutfit->Slot->NonSpawnedComponentTag.ToString(), *GetOwner()->GetName(), *OwnerComp->GetName());
					}

					if (OwnerComp->ComponentHasTag(NewOutfit->Slot->NonSpawnedComponentTag))
					{
						if (FCharacterCreationModule::CharacterCreationDebugVar > 0)
						{
							UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorComponent::SetOutfitLoadAsync: tag:[%s] Class: [%s] Found "), *NewOutfit->Slot->NonSpawnedComponentTag.ToString(), *GetOwner()->GetName());
						}

						if (NewOutfit->Slot->bSetLeaderPoseFromRoot && !NewOutfit->Slot->bIsRoot)
						{
							OwnerSkComp->SetLeaderPoseComponent(RootSkeletalMesh);
						}

						SkComp = OwnerSkComp;
						break;
					}
				}
			}
			if (!SkComp)
			{
				if (HasBegunPlay())
				{
					//We only log when the Component has begun play. This is because it might be called before the Blueprint intialization/constructor and Blueprint Created Components aren't accessible yet
					UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreatorComponent::SetOutfitLoadAsync: OufitSlot set bSpawnsComponent = false but the tag:[%s], is not present in any SkeletalMeshComponent in the Class [%s]"), *NewOutfit->Slot->NonSpawnedComponentTag.ToString(), *GetOwner()->GetName());
				}
				return;
			}
		}
	}

	if (FaceSlot && NewOutfit->Slot == FaceSlot)
	{
		FaceSkeletalMesh = SkComp;
	}

	SlotSKMeshMap.Add(NewOutfit->Slot, SkComp);
	SlotMeshMap.Add(NewOutfit->Slot, SkComp);

	const uint8 Index = static_cast<uint8>(CharacterCreator->BodyType);
	if (NewOutfit->Meshes.IsValidIndex(Index))
	{
		AssetsToStream.Add(NewOutfit->Meshes[Index].ToSoftObjectPath());

		if (NewOutfit->Slot->bRefreshGrooms)
		{
			for (const FCCSlotAndGroom& SlotAndGroom : CharacterCreator->SlotAndGroomArray)
			{
				if (SlotAndGroom.Slot == nullptr || SlotAndGroom.Groom == nullptr)
				{
					continue;
				}
				AssetsToStream.Add(SlotAndGroom.Groom->GroomAsset.ToSoftObjectPath());
				if (SlotAndGroom.Groom->BindingsMap.Contains(NewOutfit->Meshes[Index]))
				{
					AssetsToStream.Add(SlotAndGroom.Groom->BindingsMap[NewOutfit->Meshes[Index]].ToSoftObjectPath());
				}
			}
		}
	}
}

void UCharacterCreatorComponent::SetOutfitLoadedAsset(const UCharacterCreatorOutfit* NewOutfit, FName SlotName, const int32 MaterialVariantsIndex /*= 0*/)
{
	if (!NewOutfit)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreatorComponent::SetOutfitLoadedAsset: NewOufit is not valid"));
		return;
	}

	if (!RootSkeletalMesh)
	{
		//If we can't find a RootSkeletalMesh 
		UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreatorComponent::SetOutfitLoadedAsset(): Imposible to find a RootSkeletalMesh for the CharacterCreatorComponent"));
		return;
	}

	USkeletalMeshComponent* SkComp = SlotSKMeshMap.FindRef(NewOutfit->Slot);
	if (!SkComp)
	{
		if (NewOutfit->Slot->bSpawnsComponent)
		{
			USkeletalMeshComponent* CreatedSkComp = NewObject<USkeletalMeshComponent>(GetOwner(), SkeletalMeshClass, SlotName, EObjectFlags::RF_Transient);
			SkComp = CreatedSkComp;
			SkComp->ComponentTags.Add(DynamicCharacterCreatorComponentTag);
			GetOwner()->AddInstanceComponent(SkComp);
			SkComp->bUseAttachParentBound = true;
			SkComp->SetWorldTransform(FTransform::Identity);
			SkComp->AttachToComponent(RootSkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform);
			SkComp->OnComponentCreated();
			SkComp->RegisterComponent();

			CreatedSkComp->SetComponentTickEnabled(false);
			CreatedSkComp->bEnableUpdateRateOptimizations = true;
			SkComp->bReceivesDecals = false;

			for (const FName& Tag : NewOutfit->Slot->Tags)
			{
				SkComp->ComponentTags.Add(Tag);
			}
			SkComp->ComponentTags.Add(NewOutfit->Slot->NonSpawnedComponentTag); //Maybe we shouldn't include this one?
			if (NewOutfit->Slot->bSetLeaderPoseFromRoot && !NewOutfit->Slot->bIsRoot)
			{
				SkComp->SetLeaderPoseComponent(RootSkeletalMesh);
			}
		}
		else
		{
			for (UActorComponent* OwnerComp : GetOwner()->GetComponents())
			{
				if (USkeletalMeshComponent* OwnerSkComp = Cast<USkeletalMeshComponent>(OwnerComp))
				{
					if (FCharacterCreationModule::CharacterCreationDebugVar > 0)
					{
						UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorComponent::SetOutfitLoadedAsset: tag:[%s] Class: [%s] SkeletalMeshComponent to Check [%s]"), *NewOutfit->Slot->NonSpawnedComponentTag.ToString(), *GetOwner()->GetName(), *OwnerComp->GetName());
					}

					if (OwnerComp->ComponentHasTag(NewOutfit->Slot->NonSpawnedComponentTag))
					{
						if (FCharacterCreationModule::CharacterCreationDebugVar > 0)
						{
							UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorComponent::SetOutfitLoadedAsset: tag:[%s] Class: [%s] Found "), *NewOutfit->Slot->NonSpawnedComponentTag.ToString(), *GetOwner()->GetName());
						}

						if (NewOutfit->Slot->bSetLeaderPoseFromRoot && !NewOutfit->Slot->bIsRoot)
						{
							OwnerSkComp->SetLeaderPoseComponent(RootSkeletalMesh);
						}

						SkComp = OwnerSkComp;
						break;
					}
				}
			}
			if (!SkComp)
			{
				if (HasBegunPlay())
				{
					//We only log when the Component has begun play. This is because it might be called before the Blueprint intialization/constructor and Blueprint Created Components aren't accessible yet
					UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreatorComponent::SetOutfitLoadedAsset: OufitSlot set bSpawnsComponent = false but the tag:[%s], is not present in any SkeletalMeshComponent in the Class [%s]"), *NewOutfit->Slot->NonSpawnedComponentTag.ToString(), *GetOwner()->GetName());
				}
				return;
			}
		}
	}

	if (FaceSlot && NewOutfit->Slot == FaceSlot)
	{
		FaceSkeletalMesh = SkComp;
	}

	SlotSKMeshMap.Add(NewOutfit->Slot, SkComp);
	SlotMeshMap.Add(NewOutfit->Slot, SkComp);

	const uint8 Index = static_cast<uint8>(CharacterCreator->BodyType);
	if (NewOutfit->Meshes.IsValidIndex(Index))
	{
		USkeletalMesh* Skeletal = NewOutfit->Meshes[Index].Get();

		TMap<const UCharacterCreatorPoseAttribute*, float> AffectedPoseAttributes;

		for (const FCCPoseAttributeValue& PoseAttributeValue : CharacterCreator->PoseAttributeValues)
		{
			if (PoseAttributeValue.Attribute == nullptr)
			{
				continue;
			}
			if (PoseAttributeValue.Value != 0 && PoseAttributeValue.Attribute->Slot == NewOutfit->Slot)
			{
				AffectedPoseAttributes.Add(PoseAttributeValue.Attribute, PoseAttributeValue.Value);
			}
		}

		if (AffectedPoseAttributes.IsEmpty())
		{
			DuplicatedSkeletalMeshes.Remove(NewOutfit->Slot);
			SkComp->SetSkeletalMesh(Skeletal);
		}
		else
		{
			if (HasValidUVSets(Skeletal) && (!DuplicatedSkeletalMeshes.Find(NewOutfit->Slot) || DuplicatedSkeletalMeshes[NewOutfit->Slot].Duplication == nullptr || DuplicatedSkeletalMeshes[NewOutfit->Slot].Original != Skeletal))
			{
				USkeletalMesh* Original = Skeletal;

				TArray<USkeletalMesh*> MeshesToMergeCopy;
				MeshesToMergeCopy.Add(Original);

				TArray<FSkelMeshMergeSectionMapping> SectionMappings;

				USkeletalMesh* Duplication = NewObject<USkeletalMesh>();

				FSkeletalMeshMerge Merger(Duplication, MeshesToMergeCopy, SectionMappings, 0);
				Merger.DoMerge();

				Duplication->SetSkeleton(Original->GetSkeleton());
				Duplication->SetPhysicsAsset(Original->GetPhysicsAsset());

				SkComp->SetSkeletalMesh(Duplication);

				DuplicatedSkeletalMeshes.Add(NewOutfit->Slot, FDuplicatedSkeletalMesh(Original, Duplication));
			}

			FReferenceSkeleton* ModifyRefSkeleton = &DuplicatedSkeletalMeshes[NewOutfit->Slot].Duplication->GetRefSkeleton();
			const FReferenceSkeleton* ReferenceSkeleton = &DuplicatedSkeletalMeshes[NewOutfit->Slot].Original->GetRefSkeleton();

			FReferenceSkeletonModifier SkeletonModifier(*ModifyRefSkeleton, DuplicatedSkeletalMeshes[NewOutfit->Slot].Duplication->GetSkeleton());

			TMap<FName, FTransform> PoseOffsets;

			for (auto& AffectedPoseAttributeElem : AffectedPoseAttributes)
			{
				if (AffectedPoseAttributeElem.Value < 0.f)
				{
					for (auto& PoseOffsetsElem : AffectedPoseAttributeElem.Key->MinPoseOffsets)
					{
						FTransform AttributeValue = FTransform(FRotator::ZeroRotator, PoseOffsetsElem.Value.GetLocation() * FMath::Abs(AffectedPoseAttributeElem.Value) * AffectedPoseAttributeElem.Key->Power, PoseOffsetsElem.Value.GetScale3D() * FMath::Abs(AffectedPoseAttributeElem.Value));
						PoseOffsets.Add(PoseOffsetsElem.Key, PoseOffsets.Find(PoseOffsetsElem.Key) ? PoseOffsets[PoseOffsetsElem.Key] + AttributeValue : AttributeValue);
					}
				}
				else
				{
					for (auto& PoseOffsetsElem : AffectedPoseAttributeElem.Key->MaxPoseOffsets)
					{
						FTransform AttributeValue = FTransform(FRotator::ZeroRotator, PoseOffsetsElem.Value.GetLocation() * FMath::Abs(AffectedPoseAttributeElem.Value) * AffectedPoseAttributeElem.Key->Power, PoseOffsetsElem.Value.GetScale3D() * FMath::Abs(AffectedPoseAttributeElem.Value));
						PoseOffsets.Add(PoseOffsetsElem.Key, PoseOffsets.Find(PoseOffsetsElem.Key) ? PoseOffsets[PoseOffsetsElem.Key] + AttributeValue : AttributeValue);
					}
				}
			}

			for (auto& PoseOffset : PoseOffsets)
			{
				int32 BoneIndex = ModifyRefSkeleton->FindBoneIndex(PoseOffset.Key);

				if (BoneIndex == -1)
				{
					continue;
				}

				FTransform AttributeValue = ReferenceSkeleton->GetRefBonePose()[BoneIndex] + PoseOffset.Value;
				AttributeValue.NormalizeRotation();
				SkeletonModifier.UpdateRefPoseTransform(BoneIndex, AttributeValue);
			}
		}

		SkComp->EmptyOverrideMaterials();

		if (NewOutfit->MaterialVariants.IsValidIndex(MaterialVariantsIndex))
		{
			const TArray<TSoftObjectPtr<UMaterialInterface>>& Variants = NewOutfit->MaterialVariants[MaterialVariantsIndex].Materials;

			for (int32 i = 0; i < Variants.Num(); i++)
			{
				SkComp->SetMaterial(i, Variants[i].LoadSynchronous());
			}
		}

		int32 const NumMaterials = SkComp->GetNumMaterials();
		for (int32 i = 0; i < NumMaterials; i++)
		{
			UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(SkComp->GetMaterial(i));
			if (!MID)
			{
				MID = SkComp->CreateDynamicMaterialInstance(i);
			}
			for (const FCCMaterialAttributeValue& MaterialAttributeValue : CharacterCreator->MaterialAttributeValues)
			{
				if (MaterialAttributeValue.MaterialAttribute)
				{
					MaterialAttributeValue.MaterialAttribute->SetAttributeOnMaterialInstanceDynamic(MID, MaterialAttributeValue.Value);
				}
			}
			for (const FCCVectorMaterialAttributeValue& MaterialAttributeValue : CharacterCreator->VectorMaterialAttributeValues)
			{
				if (MaterialAttributeValue.AffectedSlots.Contains(NewOutfit->Slot) && MaterialAttributeValue.MaterialAttribute)
				{
					MaterialAttributeValue.MaterialAttribute->SetAttributeOnMaterialInstanceDynamic(MID, MaterialAttributeValue.Value);
				}
			}
		}

		for (const FCharacterCreatorOutfitMask& Mask : NewOutfit->Masks)
		{
			if (USkeletalMeshComponent* MaskedSkComp = Mask.bIsRoot ? RootSkeletalMesh : SlotSKMeshMap[Mask.OutfitSlot])
			{
				UMaterialInstanceDynamic* MaterialToMask = Cast<UMaterialInstanceDynamic>(MaskedSkComp->GetMaterial(Mask.MaterialIndex));
				if (!MaterialToMask)
				{
					MaskedSkComp->CreateDynamicMaterialInstance(Mask.MaterialIndex); //It will never be executed
				}
				if (MaterialToMask)
				{
					MaterialToMask->SetTextureParameterValue(Mask.ParameterName, Mask.TextureMask);
				}
			}
		}

		//Set morphs in this Outfit
		for (const FCharacterCreatorOutfitMorphAttribute& MorphAttribute : NewOutfit->Morphs)
		{
			if (MorphAttribute.OutfitSlot == nullptr)
			{
				continue;
			}
			if (USkeletalMeshComponent* MorphedSkComp = SlotSKMeshMap[MorphAttribute.OutfitSlot])
			{
				MorphedSkComp->SetMorphTarget(MorphAttribute.MorphName, MorphAttribute.Value);
			}
		}

		for (const FCCMOUnderConditions& ModifierWithConditions : NewOutfit->ModifiersWithConditions)
		{
			bool bAllConditionsMet = true;
			for (const UCharacterCreatorCondition* Condition : ModifierWithConditions.Conditions)
			{
				if (!Condition)
				{
					continue;
				}
				if (!Condition->IsConditionMet(NewOutfit, CharacterCreator))
				{
					bAllConditionsMet = false;
				}
			}
			if (!ModifierWithConditions.Modifier)
			{
				continue;
			}
			if (!bAllConditionsMet)
			{
				continue;
			}
			ModifierWithConditions.Modifier->PerformModification(this, SkComp, NewOutfit, CharacterCreator);
		}

		//Set morphs pointing to this slot in the rest of the outfits
		for (const FCCSlotAndOutfit& SlotAndOutfit : CharacterCreator->SlotAndOutfitArray)
		{
			if (SlotAndOutfit.Slot == nullptr || SlotAndOutfit.Outfit == nullptr)
			{
				continue;
			}
			for (const FCharacterCreatorOutfitMorphAttribute& MorphAttribute : SlotAndOutfit.Outfit->Morphs)
			{
				
				if (MorphAttribute.OutfitSlot != NewOutfit->Slot)
				{
					continue;
				}
				SkComp->SetMorphTarget(MorphAttribute.MorphName, MorphAttribute.Value);
			}
		}

		//if (RootSkeletalMesh)
		//{
			//We first reset all the modifiers
			//if (CharacterCreator->Model)
			//{
			//	for (const FCharacterCreatorOutfitModifier& Modifier : CharacterCreator->Model->DefaultModifiers)
			//	{
			//		RootSkeletalMesh->SetMorphTarget(Modifier.MorphTargetName, Modifier.Value);
			//	}
			//}
			//Then we apply the Outfit Modifiers
			//for (const FCharacterCreatorOutfitModifier& Modifier : NewOutfit->Modifiers)
			//{
			//	RootSkeletalMesh->SetMorphTarget(Modifier.MorphTargetName, Modifier.Value);
			//}
		//}

		if (NewOutfit->Slot->bRefreshGrooms)
		{
			for (const FCCSlotAndGroom& SlotAndGroom : CharacterCreator->SlotAndGroomArray)
			{
				if (SlotAndGroom.Slot == nullptr)
				{
					continue;
				}
				SetGroomLoadedAsset(SlotAndGroom.Groom, FName(SlotAndGroom.Slot->Name));
			}
		}
		OnOutfitChanged.Broadcast(SkComp);
	}
}

void UCharacterCreatorComponent::SetGroomLoadedAsset(const UCharacterCreatorGroom* NewGroom, const FName& SlotName)
{
	if (!NewGroom)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreatorComponent::SetGroom - NewGroom is not valid for Slot [%s]"), *SlotName.ToString());
		return;
	}
	bool bFiltered = false;
	UGroomComponent* GroomComp = FindOrCreateGroomComponent(NewGroom, SlotName, bFiltered);
	if (!GroomComp)
	{
		if (!bFiltered)
		{
			UE_LOG(
				CharacterCreationLog,
				Error,
				TEXT("UCharacterCreatorComponent::SetGroom - GroomComp is not valid. Groom: [%s] Actor: [%s],  Slot [%s]"),
				*NewGroom->GetName(),
				*GetOwner()->GetName(),
				*SlotName.ToString()
			);
		}
		return;
	}
	GroomComp->SetGroomAsset(nullptr);
	GroomComp->SetBindingAsset(nullptr);

	if (NewGroom->GroomAsset.IsNull())
	{
		return;
	}

	// Set up groom binding
	USkeletalMeshComponent* GroomAttachmentSKMeshComponent = Cast<USkeletalMeshComponent>(SlotMeshMap.FindRef(NewGroom->SlotToAttach));
	if (!GroomAttachmentSKMeshComponent)
	{
		UE_LOG(
			CharacterCreationLog,
			Log,
			TEXT("UCharacterCreatorComponent::SetGroom - No GroomAttachmentSKMeshComponent found in SlotMeshMap for Slot [%s]"),
			*SlotName.ToString());
		return;
	}

	const USkeletalMesh* SkeletalMesh = DuplicatedSkeletalMeshes.Find(NewGroom->SlotToAttach) != nullptr ? DuplicatedSkeletalMeshes[NewGroom->SlotToAttach].Original : GroomAttachmentSKMeshComponent->GetSkeletalMeshAsset();
	if (!SkeletalMesh)
	{
		UE_LOG(
			CharacterCreationLog,
			Log,
			TEXT("UCharacterCreatorComponent::SetGroom - No SkeletalMesh found in GroomAttachmentSKMeshComponent:  Groom: [%s], Actor: [%s], GroomAttachmentSKMeshComponent: [%s], Slot [%s]"),
			*NewGroom->GetName(),
			*GetOwner()->GetName(),
			*GroomAttachmentSKMeshComponent->GetName(),
			*SlotName.ToString()
		);
		return;
	}
	if (!NewGroom->BindingsMap.Contains(SkeletalMesh))
	{
		UE_LOG(
			CharacterCreationLog,
			Log,
			TEXT("UCharacterCreatorComponent::SetGroom - Groom has no associated GroomBindingAsset for USkeletalMesh. Groom: [%s], Actor: [%s], GroomAttachmentSKMeshComponent: [%s], SkeletalMesh: [%s] Slot [%s]"),
			*NewGroom->GetName(),
			*GetOwner()->GetName(),
			*GroomAttachmentSKMeshComponent->GetName(),
			*SkeletalMesh->GetName(),
			*SlotName.ToString()
		);
		return;
	}


	const auto SoftGroomBinding = NewGroom->BindingsMap.Find(SkeletalMesh);
	if (!SoftGroomBinding)
	{
		UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorComponent::SetGroom - No SoftGroomBinding found"));
		return;
	}

	UGroomAsset* GroomAsset = NewGroom->GroomAsset.Get();
	UGroomBindingAsset* GroomBinding = SoftGroomBinding->Get();
	if (GroomAsset == nullptr || GroomBinding == nullptr)
	{
		return;
	}

	TArray<FHairGroupsMaterial> GroomMaterials = GroomAsset->GetHairGroupsMaterials();
	for (int32 Index = 0; Index < GroomMaterials.Num(); Index++)
	{
		GroomComp->SetMaterial(Index, GroomMaterials[Index].Material);
	}

	GroomComp->SetBindingAsset(GroomBinding);
	GroomComp->SetGroomAsset(GroomAsset);
	GroomComp->AttachmentName = NewGroom->Slot->AttachementName;
	GroomComp->AttachToComponent(GroomAttachmentSKMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	int32 const NumMaterials = GroomComp->GetNumMaterials();
	for (int32 i = 0; i < NumMaterials; i++)
	{
		UMaterialInstanceDynamic* MID = GroomComp->CreateDynamicMaterialInstance(i);
		for (const FCCMaterialAttributeValue& MaterialAttributeValue : CharacterCreator->MaterialAttributeValues)
		{
			if (MaterialAttributeValue.MaterialAttribute)
			{
				MaterialAttributeValue.MaterialAttribute->SetAttributeOnMaterialInstanceDynamic(MID, MaterialAttributeValue.Value);
			}
		}
		for (const FCCVectorMaterialAttributeValue& MaterialAttributeValue : CharacterCreator->VectorMaterialAttributeValues)
		{
			if (MaterialAttributeValue.AffectedSlots.Contains(NewGroom->Slot) && MaterialAttributeValue.MaterialAttribute)
			{
				MaterialAttributeValue.MaterialAttribute->SetAttributeOnMaterialInstanceDynamic(MID, MaterialAttributeValue.Value);
			}
		}
	}
	SlotGroomMap.Add(NewGroom->Slot, GroomComp);
	SlotMeshMap.Add(NewGroom->Slot, GroomComp);

	for (const FCCTextureMaterialAttributeValue& TextureMaterialAttributeValue : NewGroom->TextureMaterialAttributeValues)
	{
		if (!TextureMaterialAttributeValue.TextureMaterialAttribute && !TextureMaterialAttributeValue.Value)
		{
			continue;
		}
		for (const UCharacterCreatorOutfitSlot* CharacterCreatorOutfitSlot : TextureMaterialAttributeValue.AffectedSlots)
		{
			USkeletalMeshComponent* SKMeshComponent = Cast<USkeletalMeshComponent>(SlotMeshMap.FindRef(CharacterCreatorOutfitSlot));
			if (!SKMeshComponent)
			{
				continue;
			}
			int32 const NumTextureMaterials = SKMeshComponent->GetNumMaterials();
			for (int32 i = 0; i < NumTextureMaterials; i++)
			{
				UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(SKMeshComponent->GetMaterial(i));
				if (!MID)
				{
					MID = SKMeshComponent->CreateDynamicMaterialInstance(i);
				}
				TextureMaterialAttributeValue.TextureMaterialAttribute->SetAttributeOnMaterialInstanceDynamic(MID, TextureMaterialAttributeValue.Value);
			}
		}
	}

	OnGroomChanged.Broadcast(GroomComp);
}

bool UCharacterCreatorComponent::HasValidUVSets(const USkeletalMesh* SkeletalMesh)
{
	if (!SkeletalMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("SkeletalMesh pointer is null."));
		return false;
	}

	// Get the render data for the skeletal mesh
	FSkeletalMeshRenderData* RenderData = SkeletalMesh->GetResourceForRendering();
	if (!RenderData)
	{
		UE_LOG(LogTemp, Error, TEXT("SkeletalMesh render data is null."));
		return false;
	}

	// Check each LOD for the correct number of UV sets
	for (const FSkeletalMeshLODRenderData& LODData : RenderData->LODRenderData)
	{
		int32 NumUVs = LODData.StaticVertexBuffers.StaticMeshVertexBuffer.GetNumTexCoords();
		if (NumUVs < 0 || NumUVs > 4)
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid number of UV sets: %d. Must be between 0 and 4."), NumUVs);
			return false;
		}
	}

	// All LODs have a valid number of UV sets.
	return true;
}

void UCharacterCreatorComponent::OnOutfitChangedReceived(const UCharacterCreatorOutfit* Outfit, FName SlotName, const int32 MaterialVariantIndex /*= -1*/)
{
	Multicast_OnOutfitChangedReceived(Outfit, SlotName, MaterialVariantIndex);
}

void UCharacterCreatorComponent::Multicast_OnOutfitChangedReceived_Implementation(const UCharacterCreatorOutfit* Outfit, FName SlotName, const int32 MaterialVariantIndex /*= -1*/)
{
	SetOutfit(Outfit, SlotName, MaterialVariantIndex);
}

void UCharacterCreatorComponent::OnGroomChangedReceived(const UCharacterCreatorGroom* Groom, FName SlotName)
{
	Multicast_OnGroomChangedReceived(Groom, SlotName);
}

void UCharacterCreatorComponent::Multicast_OnGroomChangedReceived_Implementation(const UCharacterCreatorGroom* Groom, FName SlotName)
{
	SetGroom(Groom, SlotName);
}

void UCharacterCreatorComponent::OnAttributeChangedReceived(const UCharacterCreatorAttribute* Attribute, float Value)
{
	Multicast_OnAttributeChangedReceived(Attribute, Value);
}

void UCharacterCreatorComponent::Multicast_OnAttributeChangedReceived_Implementation(const UCharacterCreatorAttribute* Attribute, float Value)
{
	SetMorphTarget(Attribute, Value);
}

void UCharacterCreatorComponent::OnMaterialAttributeChangedReceived(const UCharacterCreatorMatAttribute* MaterialAttribute, float Value)
{
	Multicast_OnMaterialAttributeChangedReceived(MaterialAttribute, Value);
}

void UCharacterCreatorComponent::Multicast_OnMaterialAttributeChangedReceived_Implementation(const UCharacterCreatorMatAttribute* MaterialAttribute, float Value)
{
	SetMaterialAttribute(MaterialAttribute, Value);
}

void UCharacterCreatorComponent::OnVectorMaterialAttributeChangedReceived(const UCharacterCreatorVectorMatAttribute* MaterialAttribute, FLinearColor Value)
{
	Multicast_OnVectorMaterialAttributeChangedReceived(MaterialAttribute, Value);
}

void UCharacterCreatorComponent::Multicast_OnVectorMaterialAttributeChangedReceived_Implementation(const UCharacterCreatorVectorMatAttribute* MaterialAttribute, FLinearColor Value)
{
	SetVectorMaterialAttribute(MaterialAttribute, Value);
}

void UCharacterCreatorComponent::OnPoseAttributeChangedReceived(const UCharacterCreatorPoseAttribute* PoseAttribute, float Value)
{
	Multicast_OnPoseAttributeChangedReceived(PoseAttribute, Value);
}

void UCharacterCreatorComponent::Multicast_OnPoseAttributeChangedReceived_Implementation(const UCharacterCreatorPoseAttribute* PoseAttribute, float Value)
{
	SetPoseAttribute(PoseAttribute, Value);
}

void UCharacterCreatorComponent::SetMorphTarget(const UCharacterCreatorAttribute* Attribute, const float& Value) const
{
	if (RootSkeletalMesh && Attribute && RootSkeletalMesh->FindMorphTarget(Attribute->MorphName))
	{
		RootSkeletalMesh->SetMorphTarget(Attribute->MorphName, Value);
	}
}

void UCharacterCreatorComponent::OnBodyTypeChangedReceived(FCharacterCreationBodyType NewBodyType)
{
	CharacterCreator->BodyType = NewBodyType;

	for (const FCCSlotAndOutfit& SlotAndOutfit : CharacterCreator->SlotAndOutfitArray)
	{
		SetOutfit(SlotAndOutfit.Outfit, FName(SlotAndOutfit.Slot->Name), SlotAndOutfit.MaterialVariantIndex);
	}
}

void UCharacterCreatorComponent::SetDefaultRootSkeletalMesh()
{
	if (!CharacterCreator)
	{
		return;
	}

	//We first try to get a component that has the Tag defined in this CharacterCreator

	for (UActorComponent* OwnerComp : GetOwner()->GetComponents())
	{
		if (USkeletalMeshComponent* OwnerSkComp = Cast<USkeletalMeshComponent>(OwnerComp))
		{
			if (OwnerSkComp->ComponentHasTag(CharacterCreator->RootSkeletalMeshTag))
			{
				RootSkeletalMesh = OwnerSkComp;
				return;
			}
		}
	}

	//Secondly we try to find just one Skeletal Mesh Component with no parent
	for (UActorComponent* OwnerComp : GetOwner()->GetComponents())
	{
		if (USkeletalMeshComponent* OwnerSkComp = Cast<USkeletalMeshComponent>(OwnerComp))
		{
			if (OwnerSkComp->GetAttachParent() != nullptr && OwnerSkComp->GetAttachParent()->IsA(USkeletalMeshComponent::StaticClass()) == false)
			{
				RootSkeletalMesh = OwnerSkComp;
				return;
			}
		}
	}
	//If that didn't work, we better configure it better
	UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreatorComponent::SetDefaultRootSkeletalMesh(): Imposible to find a RootSkeletalMesh for the CharacterCreatorComponent"));
}
