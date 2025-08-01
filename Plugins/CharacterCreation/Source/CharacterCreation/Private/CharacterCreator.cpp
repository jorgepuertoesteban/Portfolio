// Created by Jorge Puerto. All Rights Reserved.


#include "CharacterCreator.h"
#include "CharacterCreationTypes.h"
#include "CharacterCreatorAttribute.h"
#include "CharacterCreatorMatAttribute.h"
#include "CharacterCreatorVectorMatAttribute.h"
#include "CharacterCreatorPoseAttribute.h"
#include "CharacterCreatorOutfit.h"
#include "CharacterCreatorGroom.h"
#include "CharacterCreatorOutfitSlot.h"
#include "Net/UnrealNetwork.h"
#include "Logs/CharacterCreationLog.h"

void UCharacterCreator::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterCreator, SlotAndOutfitArray);
	DOREPLIFETIME(UCharacterCreator, SlotAndGroomArray);
	DOREPLIFETIME(UCharacterCreator, AttributeValues);
	DOREPLIFETIME(UCharacterCreator, MaterialAttributeValues);
	DOREPLIFETIME(UCharacterCreator, PoseAttributeValues);
	DOREPLIFETIME(UCharacterCreator, VectorMaterialAttributeValues);
	DOREPLIFETIME(UCharacterCreator, BodyType);
	DOREPLIFETIME(UCharacterCreator, Model);
}

int32 UCharacterCreator::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	if (HasAnyFlags(RF_ClassDefaultObject) || !IsSupportedForNetworking())
	{
		// This handles absorbing authority/cosmetic
		return GEngine->GetGlobalFunctionCallspace(Function, this, Stack);
	}
	check(GetOuter() != nullptr);
	return GetOuter()->GetFunctionCallspace(Function, Stack);
}

bool UCharacterCreator::CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack)
{
	check(!HasAnyFlags(RF_ClassDefaultObject));
	check(GetOuter() != nullptr);

	AActor* Owner = CastChecked<AActor>(GetOuter());

	bool bProcessed = false;

	FWorldContext* const Context = GEngine->GetWorldContextFromWorld(GetWorld());
	if (Context != nullptr)
	{
		for (FNamedNetDriver& Driver : Context->ActiveNetDrivers)
		{
			if (Driver.NetDriver != nullptr && Driver.NetDriver->ShouldReplicateFunction(Owner, Function))
			{
				Driver.NetDriver->ProcessRemoteFunction(Owner, Function, Parameters, OutParms, Stack, this);
				bProcessed = true;
			}
		}
	}
	return bProcessed;
}

UWorld* UCharacterCreator::GetWorld() const
{
	if (HasAllFlags(RF_ClassDefaultObject))
	{
		// If we are a CDO, we must return nullptr instead of calling Outer->GetWorld() to fool UObject::ImplementsGetWorld.
		return nullptr;
	}
	return GetOuter()->GetWorld();
}

float UCharacterCreator::ValueOf(const UCharacterCreatorAttribute* CCAttribute)
{
	for (const FCCAttributeValue& AttributeValue : AttributeValues)
	{
		if (AttributeValue.Attribute == CCAttribute)
		{
			return AttributeValue.Value;
		}
	}
	return 0.f;
}

//Quick fix for material attribute testing, TODO:Interface/Hierarchy the attributes so they share types
float UCharacterCreator::ValueOf(const UCharacterCreatorMatAttribute* CCAttribute)
{
	for (const FCCMaterialAttributeValue& MaterialAttributeValue : MaterialAttributeValues)
	{
		if (MaterialAttributeValue.MaterialAttribute == CCAttribute)
		{
			return MaterialAttributeValue.Value;
		}
	}
	return 0.f;
}


void UCharacterCreator::ClearCharacterCreator()
{
	SlotAndOutfitArray.Empty();
	SlotAndGroomArray.Empty();
	AttributeValues.Empty();
	MaterialAttributeValues.Empty();
}

void UCharacterCreator::SetAttributeValue(const UCharacterCreatorAttribute* CCAttribute, float NewValue)
{
	bool bIsFound = false;

	for (FCCAttributeValue& AttributeValue : AttributeValues)
	{
		if (AttributeValue.Attribute == CCAttribute)
		{
			AttributeValue.Value = NewValue;
			bIsFound = true;
			break;
		}
	}
	if (!bIsFound)
	{
		AttributeValues.Emplace(CCAttribute, NewValue); //We create a new entry otherwise
	}

	Multicast_AttributeChanged(CCAttribute, NewValue);
}

void UCharacterCreator::Multicast_AttributeChanged_Implementation(const UCharacterCreatorAttribute* Attribute, float NewValue)
{
	UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreator::Multicast_AttributeChanged_Implementation() Called, Attribute: %s and Value: %f"), *Attribute->GetName(), NewValue);
	OnAttributeChangedDelegate.Broadcast(Attribute, NewValue);
}

void UCharacterCreator::SetMaterialAttributeValue(const UCharacterCreatorMatAttribute* CCAttribute, float NewValue)
{
	bool bIsFound = false;

	for (FCCMaterialAttributeValue& AttributeValue : MaterialAttributeValues)
	{
		if (AttributeValue.MaterialAttribute == CCAttribute)
		{
			AttributeValue.Value = NewValue;
			bIsFound = true;

			break;
		}
	}
	if (!bIsFound)
	{
		MaterialAttributeValues.Emplace(CCAttribute, NewValue); //We create a new entry otherwise
	}

	Multicast_MaterialAttributeChanged(CCAttribute, NewValue);
}

void UCharacterCreator::Multicast_MaterialAttributeChanged_Implementation(const UCharacterCreatorMatAttribute* MaterialAttribute, float NewValue)
{
	UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreator::Multicast_AttributeChanged_Implementation() Called, Attribute: %s and Value: %f"), *MaterialAttribute->GetName(), NewValue);

	OnMaterialAttributeChangedDelegate.Broadcast(MaterialAttribute, NewValue);
}

void UCharacterCreator::SetVectorMaterialAttributeValue(const UCharacterCreatorVectorMatAttribute* CCAttribute, FLinearColor NewValue)
{
	bool bIsFound = false;

	for (FCCVectorMaterialAttributeValue& AttributeValue : VectorMaterialAttributeValues)
	{
		if (AttributeValue.MaterialAttribute == CCAttribute)
		{
			AttributeValue.Value = NewValue;
			bIsFound = true;

			break;
		}
	}
	if (!bIsFound)
	{
		VectorMaterialAttributeValues.Emplace(CCAttribute, NewValue);
	}

	Multicast_VectorMaterialAttributeChanged(CCAttribute, NewValue);
}

void UCharacterCreator::Multicast_VectorMaterialAttributeChanged_Implementation(const UCharacterCreatorVectorMatAttribute* MaterialAttribute, FLinearColor NewValue)
{
	UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreator::Multicast_VectorMaterialAttributeChanged_Implementation() Called, Attribute: %s"), *MaterialAttribute->GetName());

	OnVectorMaterialAttributeChangedDelegate.Broadcast(MaterialAttribute, NewValue);
}

void UCharacterCreator::SetPoseAttributeValue(const UCharacterCreatorPoseAttribute* CCAttribute, float NewValue)
{
	bool bIsFound = false;

	for (FCCPoseAttributeValue& AttributeValue : PoseAttributeValues)
	{
		if (AttributeValue.Attribute == CCAttribute)
		{
			AttributeValue.Value = NewValue;
			bIsFound = true;

			break;
		}
	}
	if (!bIsFound)
	{
		PoseAttributeValues.Emplace(CCAttribute, NewValue); //We create a new entry otherwise
	}

	Multicast_PoseAttributeChanged(CCAttribute, NewValue);
}

void UCharacterCreator::Multicast_PoseAttributeChanged_Implementation(const UCharacterCreatorPoseAttribute* PoseAttribute, float NewValue)
{
	UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreator::Multicast_PoseAttributeChanged_Implementation() Called, Attribute: %s and Value: %f"), *PoseAttribute->GetName(), NewValue);

	OnPoseAttributeChangedDelegate.Broadcast(PoseAttribute, NewValue);
}

//TODO: Change to separate functions, add and remove, as this functionality is redundant when sharing slot with other MaterialAttributes
void UCharacterCreator::SetMaterialAttributeAffectedSlots(const UCharacterCreatorMatAttribute* CCAttribute, const UCharacterCreatorOutfitSlot* Slot, bool bIsChecked)
{
	for (FCCMaterialAttributeValue& AttributeValue : MaterialAttributeValues)
	{
		if (AttributeValue.MaterialAttribute == CCAttribute)
		{
			//SetMaterialAttributeAffectedSlots(AttributeValue.MaterialAttribute, 
			if (bIsChecked)
			{
				//This condition should never be true, the statement is just for security
				if (!AttributeValue.AffectedSlots.Contains(Slot))
				{
					AttributeValue.AffectedSlots.Emplace(Slot);

					for (FCCMaterialAttributeValue& SharedSlotAttributeValue : MaterialAttributeValues)
					{
						if (SharedSlotAttributeValue.MaterialAttribute->TargetSlot == Slot)
						{
							SetMaterialAttributeAffectedSlots(SharedSlotAttributeValue.MaterialAttribute, AttributeValue.MaterialAttribute->TargetSlot, bIsChecked);
						}
					}
				}
			}
			else
			{
				//This condition should never be true, the statement is just for security
				if (AttributeValue.AffectedSlots.Contains(Slot))
				{
					AttributeValue.AffectedSlots.Remove(Slot);

					for (FCCMaterialAttributeValue& SharedSlotAttributeValue : MaterialAttributeValues)
					{
						if (SharedSlotAttributeValue.MaterialAttribute->TargetSlot == Slot)
						{
							SetMaterialAttributeAffectedSlots(SharedSlotAttributeValue.MaterialAttribute, AttributeValue.MaterialAttribute->TargetSlot, bIsChecked);
						}
					}
				}
			}

			break;
		}
	}

	Multicast_MaterialAttributeChanged(CCAttribute, bIsChecked);
}

//Quick fix for material attribute testing, TODO:Interface/Hierarchy the attributes so they share types
void UCharacterCreator::Multicast_MaterialAttributeAffectedSlotChanged_Implementation(const UCharacterCreatorMatAttribute* CCAttribute, const UCharacterCreatorOutfitSlot* Slot, bool NewValue)
{
	UE_LOG(CharacterCreationLog, Log, TEXT("UCharacterCreator::Multicast_MaterialAttributeAffectedSlotChanged_Implementation() Called, Attribute: %s and Value: %s"), *CCAttribute->GetName(), NewValue?TEXT("TRUE"):TEXT("FALSE"));

	if (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer)
	{
		UE_LOG(CharacterCreationLog, Log, TEXT("I'm Server"));
	}
	else
	{
		UE_LOG(CharacterCreationLog, Log, TEXT("I'm Client"));
	}
	//OnMaterialAttributeChangedDelegate.Broadcast(MaterialAttribute, NewValue);
}

const UCharacterCreatorOutfit* UCharacterCreator::GetSelectedOutfit(const UCharacterCreatorOutfitSlot* Slot) const
{
	for (const FCCSlotAndOutfit& SlotAndOutfit : SlotAndOutfitArray)
	{
		if (SlotAndOutfit.Slot == Slot)
		{
			return SlotAndOutfit.Outfit;
		}
	}
	return nullptr;
}

void UCharacterCreator::SetOutfit(const UCharacterCreatorOutfit* Outfit, const int32 MaterialVariantIndex /*= INDEX_NONE*/)
{
	bool bIsFound = false;

	if (!Outfit || !Outfit->Slot)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreator::SetOutfit Oufit or Outfit->Slot is/are null"));
		return;
	}

	for (FCCSlotAndOutfit& SlotAndOutfit : SlotAndOutfitArray)
	{
		if (SlotAndOutfit.Slot == Outfit->Slot)
		{
			SlotAndOutfit.Outfit = Outfit;
			bIsFound = true;
			SlotAndOutfit.MaterialVariantIndex = MaterialVariantIndex;
			break;
		}
	}
	if (!bIsFound)
	{
		SlotAndOutfitArray.Emplace(Outfit->Slot, Outfit, MaterialVariantIndex); //We create a new entry otherwise
	}

	Multicast_OutfitChanged(Outfit, FName(Outfit->Slot->Name), MaterialVariantIndex);
}

bool UCharacterCreator::IsUsingOutfit(const UCharacterCreatorOutfit* Outfit, const int32 MaterialVariantIndex  /*= INDEX_NONE*/) const
{
	if (Outfit == nullptr)
	{
		return false;
	}
	for (const FCCSlotAndOutfit& SlotAndOutfit : SlotAndOutfitArray)
	{
		if (SlotAndOutfit.Outfit == Outfit && (MaterialVariantIndex == INDEX_NONE || SlotAndOutfit.MaterialVariantIndex == MaterialVariantIndex))
		{
			return true;
		}
	}
	return false;
}

void UCharacterCreator::ClearOutfitSlot(const UCharacterCreatorOutfitSlot* OutfitSlot)
{
	bool bIsFound = false;
	if (!OutfitSlot)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreator::ClearOutfitSlot OutfitSlot is null"));
		return;
	}
	for (FCCSlotAndOutfit& SlotAndOutfit : SlotAndOutfitArray)
	{
		if (SlotAndOutfit.Slot == OutfitSlot)
		{
			SlotAndOutfit.Outfit = OutfitSlot->DefaultOutfit;
			SlotAndOutfit.MaterialVariantIndex = INDEX_NONE;
			bIsFound = true;
			break;
		}
	}
	if (!bIsFound)
	{
		SlotAndOutfitArray.Emplace(OutfitSlot, OutfitSlot->DefaultOutfit, INDEX_NONE); 
	}

	Multicast_OutfitChanged(OutfitSlot->DefaultOutfit, FName(OutfitSlot->Name), INDEX_NONE);
}

void UCharacterCreator::Multicast_OutfitChanged_Implementation(const UCharacterCreatorOutfit* Outfit, FName SlotName, const int32 MaterialVariantIndex /*= INDEX_NONE*/)
{
	OnOutfitChangedDelegate.Broadcast(Outfit, SlotName, MaterialVariantIndex);
}

const UCharacterCreatorGroom* UCharacterCreator::GetSelectedGroom(const UCharacterCreatorOutfitSlot* Slot)
{
	for (const FCCSlotAndGroom& SlotAndGroom : SlotAndGroomArray)
	{
		if (SlotAndGroom.Slot == Slot)
		{
			return SlotAndGroom.Groom;
		}
	}
	return nullptr;
}

void UCharacterCreator::SetGroom(const UCharacterCreatorGroom* Groom)
{
	bool bIsFound = false;
	if (Groom && Groom->Slot)
	{
		for (FCCSlotAndGroom& SlotAndGroom : SlotAndGroomArray)
		{
			if (SlotAndGroom.Slot == Groom->Slot)
			{
				SlotAndGroom.Groom = Groom;
				bIsFound = true;
				break;
			}
		}
	}
	if (!bIsFound)
	{
		SlotAndGroomArray.Emplace(Groom->Slot, Groom); //We create a new entry otherwise
	}
	Multicast_GroomChanged(Groom, FName(Groom->Slot->Name));
}

void UCharacterCreator::Multicast_GroomChanged_Implementation(const UCharacterCreatorGroom* Groom, FName SlotName)
{
	OnGroomChangedDelegate.Broadcast(Groom, SlotName);
}


FCharacterCreationBodyType UCharacterCreator::GetSelectedBodyType() const
{
	return BodyType;
}

void UCharacterCreator::SetCharacterCreatorMaterialVariantIndex(int32 NewBodyType)
{
}

void UCharacterCreator::Multicast_CharacterCreatorMaterialVariantIndex_Implementation(int32 NewIndex)
{
}

void UCharacterCreator::SetBodyType(FCharacterCreationBodyType NewBodyType)
{
	BodyType = NewBodyType;
	Multicast_BodyTypeChanged(NewBodyType);
}

void UCharacterCreator::Multicast_BodyTypeChanged_Implementation(FCharacterCreationBodyType NewBodyType)
{
	OnBodyTypeChangedDelegate.Broadcast(NewBodyType);
}
