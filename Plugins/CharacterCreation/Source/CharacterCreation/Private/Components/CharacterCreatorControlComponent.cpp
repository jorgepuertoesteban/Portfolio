// Created by Jorge Puerto. All Rights Reserved.

#include "Components/CharacterCreatorControlComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "CharacterCreator.h"
#include "CharacterCreatorGroom.h"
#include "CharacterCreatorOutfit.h"
#include "GroomBindingAsset.h"
#include "GroomAsset.h"
#include "GroomComponent.h"
#include "CharacterCreatorPawn.h"
#include "Components/CharacterCreatorComponent.h"
#include "Components/CharacterCreatorHUDComponent.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/HUD.h"
#include "Net/UnrealNetwork.h"
#include "Interfaces/CharacterCreationDAO.h"
#include "Interfaces/CharacterCreatorControlComponentOwner.h"
#include "Logs/CharacterCreationLog.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

UCharacterCreatorControlComponent* UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(const AActor* Owner)
{
	if (!Owner)
	{
		return nullptr;
	}
#ifdef JPE_FORCE_CAST
	return Cast<ICharacterCreatorControlComponentOwner>(Owner)->GetCharacterCreatorControlComponent();
#else
	return Owner->FindComponentByClass<UCharacterCreatorControlComponent>();
#endif
}

void UCharacterCreatorControlComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterCreatorControlComponent, CharacterCreators);
	DOREPLIFETIME(UCharacterCreatorControlComponent, MainCharacterCreator);
}

void UCharacterCreatorControlComponent::AddCharacterCreator(UCharacterCreator* NewCharacterCreator, bool bIsMainCC /*= true*/)
{
	CharacterCreators.Add(NewCharacterCreator);
	if (bIsMainCC)
	{
		MainCharacterCreator = NewCharacterCreator;
	}
}


void UCharacterCreatorControlComponent::SetAttributeValue(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorAttribute* CCAttribute, float NewValue)
{
	Server_SetAttributeValue(NewCharacterCreator, CCAttribute, NewValue);
}

void UCharacterCreatorControlComponent::Server_SetAttributeValue_Implementation(UCharacterCreator* CharacterCreator, const UCharacterCreatorAttribute* CCAttribute, float NewValue)
{
	if (CharacterCreator && CharacterCreators.Contains(CharacterCreator))
	{
		CharacterCreator->SetAttributeValue(CCAttribute, NewValue);
	}
}

void UCharacterCreatorControlComponent::SetMaterialAttributeValue(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorMatAttribute* CCAttribute, float NewValue)
{
	Server_SetMaterialAttributeValue(NewCharacterCreator, CCAttribute, NewValue);
}

void UCharacterCreatorControlComponent::Server_SetMaterialAttributeValue_Implementation(UCharacterCreator* CharacterCreator, const UCharacterCreatorMatAttribute* CCAttribute, float NewValue)
{
	if (CharacterCreator && CharacterCreators.Contains(CharacterCreator))
	{
		CharacterCreator->SetMaterialAttributeValue(CCAttribute, NewValue);
	}
}

void UCharacterCreatorControlComponent::SetVectorMaterialAttributeValue(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorVectorMatAttribute* CCAttribute, FLinearColor NewValue)
{
	Server_SetVectorMaterialAttributeValue(NewCharacterCreator, CCAttribute, NewValue);
}

void UCharacterCreatorControlComponent::Server_SetVectorMaterialAttributeValue_Implementation(UCharacterCreator* CharacterCreator, const UCharacterCreatorVectorMatAttribute* CCAttribute, FLinearColor NewValue)
{
	if (CharacterCreator && CharacterCreators.Contains(CharacterCreator))
	{
		CharacterCreator->SetVectorMaterialAttributeValue(CCAttribute, NewValue);
	}
}

void UCharacterCreatorControlComponent::SetPoseAttributeValue(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorPoseAttribute* CCPoseAttribute, float NewValue)
{
	Server_SetPoseAttributeValue(NewCharacterCreator, CCPoseAttribute, NewValue);
}

void UCharacterCreatorControlComponent::Server_SetPoseAttributeValue_Implementation(UCharacterCreator* CharacterCreator, const UCharacterCreatorPoseAttribute* CCPoseAttribute, float NewValue)
{
	if (CharacterCreator && CharacterCreators.Contains(CharacterCreator))
	{
		CharacterCreator->SetPoseAttributeValue(CCPoseAttribute, NewValue);
	}
}

void UCharacterCreatorControlComponent::SetMaterialAttributeAffectedSlots(UCharacterCreator* NewCharacterCreator, const UCharacterCreatorMatAttribute* CCAttribute, const UCharacterCreatorOutfitSlot* Slot, bool NewValue)
{
	Server_SetMaterialAttributeAffectedSlots(NewCharacterCreator, CCAttribute, Slot, NewValue);
}

void UCharacterCreatorControlComponent::Server_SetMaterialAttributeAffectedSlots_Implementation(UCharacterCreator* CharacterCreator, const UCharacterCreatorMatAttribute* CCAttribute, const UCharacterCreatorOutfitSlot* Slot, bool NewValue)
{
	if (CharacterCreator && CharacterCreators.Contains(CharacterCreator))
	{
		CharacterCreator->SetMaterialAttributeAffectedSlots(CCAttribute, Slot, NewValue);
	}
}

void UCharacterCreatorControlComponent::SetOutfit(UCharacterCreator* CharacterCreator, const UCharacterCreatorOutfit* SelectedOutfit, const int32 MaterialVariantIndex /*= INDEX_NONE*/)
{
	Server_SetOutfit(CharacterCreator, SelectedOutfit, MaterialVariantIndex);
}

void UCharacterCreatorControlComponent::Server_SetOutfit_Implementation(UCharacterCreator* CharacterCreator, const UCharacterCreatorOutfit* SelectedOutfit, const int32 MaterialVariantIndex /*= INDEX_NONE*/)
{
	if (CharacterCreator && CharacterCreators.Contains(CharacterCreator))
	{
		CharacterCreator->SetOutfit(SelectedOutfit, MaterialVariantIndex);
		ValidateGroomBindings(CharacterCreator, SelectedOutfit);
	}
}

void UCharacterCreatorControlComponent::ClearOutfitSlot(UCharacterCreator* CharacterCreator, const UCharacterCreatorOutfitSlot* OutfitSlot)
{
	Server_ClearOutfitSlot(CharacterCreator, OutfitSlot);
}

void UCharacterCreatorControlComponent::Server_ClearOutfitSlot_Implementation(UCharacterCreator* CharacterCreator, const UCharacterCreatorOutfitSlot* OutfitSlot)
{
	if (CharacterCreator && CharacterCreators.Contains(CharacterCreator))
	{
		CharacterCreator->ClearOutfitSlot(OutfitSlot);
	}
}

void UCharacterCreatorControlComponent::ValidateGroomBindings(UCharacterCreator* CharacterCreator, const UCharacterCreatorOutfit* SelectedOutfit)
{
	Client_ValidateGroomBindings(CharacterCreator, SelectedOutfit);
}

void UCharacterCreatorControlComponent::Client_ValidateGroomBindings_Implementation(UCharacterCreator* CharacterCreator, const UCharacterCreatorOutfit* SelectedOutfit)
{
	TArray<UGroomComponent*> GroomComponents;
	GetControlledPawn()->GetComponents<UGroomComponent>(GroomComponents);
	for (UGroomComponent* GroomComponent : GroomComponents)
	{
		for (const auto [Slot, Groom] : CharacterCreator->SlotAndGroomArray)
		{
			if (!GroomComponent->GroomAsset || !Groom->GroomAsset || GroomComponent->GroomAsset->GetAssetPathName() != Groom->GroomAsset->GetAssetPathName())
			{
				continue;
			}
			for (const TSoftObjectPtr<USkeletalMesh>& SoftMesh : SelectedOutfit->Meshes)
			{
				if (auto SoftGroomBinding = Groom->BindingsMap.Find(SoftMesh))
				{
					FSoftObjectPath SoftObjectPath = SoftGroomBinding->ToSoftObjectPath();
					UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftObjectPath, [GroomComponent, SoftGroomBinding]()
					{
						GroomComponent->SetBinding(SoftGroomBinding->Get());
					});
				}
			}
		}
	}
}

void UCharacterCreatorControlComponent::SetGroom(UCharacterCreator* CharacterCreator, const UCharacterCreatorGroom* SelectedGroom)
{
	Server_SetGroom(CharacterCreator, SelectedGroom);
}

void UCharacterCreatorControlComponent::Server_SetGroom_Implementation(UCharacterCreator* CharacterCreator, const UCharacterCreatorGroom* SelectedGroom)
{
	if (CharacterCreator && CharacterCreators.Contains(CharacterCreator))
	{
		CharacterCreator->SetGroom(SelectedGroom);
	}
}

void UCharacterCreatorControlComponent::SetBodyType(UCharacterCreator* CharacterCreator, FCharacterCreationBodyType NewBodyType)
{
	Server_SetBodyType(CharacterCreator, NewBodyType);
}

void UCharacterCreatorControlComponent::Server_SetBodyType_Implementation(UCharacterCreator* CharacterCreator, FCharacterCreationBodyType NewBodyType)
{
	if (CharacterCreator && CharacterCreators.Contains(CharacterCreator))
	{
		CharacterCreator->SetBodyType(NewBodyType);
	}
}

void UCharacterCreatorControlComponent::SaveCharacterCreator(UCharacterCreator* CharacterCreator)
{
	if (!CharacterCreator)
	{
		if (!MainCharacterCreator)
		{
			UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorControlComponent::Server_SaveCharacterCreator_Implementation() CharacterCreator is null"));
			return;
		}
		CharacterCreator = MainCharacterCreator;
	}

	if (!CharacterCreators.Contains(CharacterCreator))
	{
		UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorControlComponent::Server_SaveCharacterCreator_Implementation() CharacterCreator not found in CharacterCreators"));
		return;
	}

	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorControlComponent::Server_SaveCharacterCreator_Implementation() World is null or GameInstance is null"));
		return;
	}

	const ICharacterCreationDAOOwner* const DaoOwner = Cast<ICharacterCreationDAOOwner>(GetWorld()->GetGameInstance());
	if (!DaoOwner)
	{
		UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreatorControlComponent::Server_SaveCharacterCreator_Implementation() GameInstance is not ICharacterCreationDAOOwner"));
		return;
	}

	ICharacterCreationDAO* Dao = DaoOwner->GetCharacterCreationDAO();
	if (Dao)
	{
		FSaveCharacterCreatorDelegate Delegate;
		Delegate.BindWeakLambda(this, [this](const FSaveCharacterCreatorResponse& Response)
		{
			Client_Notify(Response.Success, Response.Message);
		});
		Dao->SaveCharacterCreator(CharacterCreator, Delegate);
	}
}

void UCharacterCreatorControlComponent::Server_SaveCharacterCreator_Implementation(UCharacterCreator* CharacterCreator)
{
	SaveCharacterCreator(CharacterCreator);
}

void UCharacterCreatorControlComponent::ShowCharacterCreatorHUD()
{
	UCharacterCreatorHUDComponent* CharacterCreatorHUDComponent = UCharacterCreatorHUDComponent::GetCharacterCreatorHUDComponent(GetHUD());
	if (!CharacterCreatorHUDComponent)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("No CharacterCreatorHUDComponent Found."));
		return;
	}
	CharacterCreatorHUDComponent->ShowCharacterCreator();
}

void UCharacterCreatorControlComponent::HideCharacterCreatorHUD()
{
	const UCharacterCreatorHUDComponent* CharacterCreatorHUDComponent = UCharacterCreatorHUDComponent::GetCharacterCreatorHUDComponent(GetHUD());
	if (!CharacterCreatorHUDComponent)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("No CharacterCreatorHUDComponent Found."));
		return;
	}
	CharacterCreatorHUDComponent->HideCharacterCreator();
}

void UCharacterCreatorControlComponent::SetCharacterCreator(UCharacterCreator* NewCharacterCreator)
{
	MainCharacterCreator = NewCharacterCreator;
	CharacterCreators.AddUnique(NewCharacterCreator);
	OnNewMainCharacterCreatorChangedEvent.Broadcast(MainCharacterCreator);
}

void UCharacterCreatorControlComponent::Server_SpawnCharacterCreatorPawn_Implementation(const FTransform& SpawnTransform, const int32 CharacterCreatorIndex)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("No valid PlayerController found"));
		return;
	}
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("No valid PlayerController found"));
		return;
	}

	UCharacterCreator* CharacterCreatorToUse = MainCharacterCreator;
	if (CharacterCreators.IsValidIndex(CharacterCreatorIndex))
	{
		CharacterCreatorToUse = CharacterCreators[CharacterCreatorIndex];
	}
	if (!CharacterCreatorToUse)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("No CharacterCreator found"));
		return;
	}
	ACharacterCreatorPawn* ResultPawn = World->SpawnActorDeferred<ACharacterCreatorPawn>(CharacterCreatorPawnClass, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!ResultPawn)
	{
		UE_LOG(CharacterCreationLog, Warning, TEXT("Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(CharacterCreatorPawnClass), *SpawnTransform.ToHumanReadableString());
		return;
	}

	ResultPawn->SetFlags(RF_Transient); // We never want to save default player pawns into a map
	ResultPawn->CharacterCreatorComponent->SetCharacterCreator(CharacterCreatorToUse);
	ResultPawn->FinishSpawning(SpawnTransform, true);
	PC->Possess(ResultPawn);
}

void UCharacterCreatorControlComponent::Server_DespawnCharacterCreatorPawn_Implementation()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("Server_DespawnCharacterCreatorPawn_Implementation::No valid PlayerController found"));
		return;
	}
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("Server_DespawnCharacterCreatorPawn_Implementation::No valid PlayerController found"));
		return;
	}
	ACharacterCreatorPawn* ResultPawn = Cast<ACharacterCreatorPawn>(PC->GetPawn());
	if (!ResultPawn)
	{
		UE_LOG(CharacterCreationLog, Warning, TEXT("Server_DespawnCharacterCreatorPawn_Implementation::Couldn't find Pawn"));
		return;
	}
	World->DestroyActor(ResultPawn);
}
