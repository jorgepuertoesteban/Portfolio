// Created by Jorge Puerto. All rights reserved.


#include "CharacterCreatorModel.h"
#include "CharacterCreatorOutfitSlot.h"

bool UCharacterCreatorModel::ContainsSlot(const UCharacterCreatorOutfitSlot* FindSlot)
{
	if (Slots.IsEmpty()) {
		return false;
	}
	return Slots.Contains(FindSlot);
}

bool UCharacterCreatorModel::ContainsSlot(const FString& SlotID)
{
	//TODO think of something better
	for (const UCharacterCreatorOutfitSlot* Slot : Slots)
	{
		if (Slot->Name == SlotID)
		{
			return true;
		}
	}
	return false;
}

const UCharacterCreatorOutfitSlot* UCharacterCreatorModel::GetSlot(const FString& SlotID)
{
	//TODO think of something better
	for (const UCharacterCreatorOutfitSlot* Slot : Slots)
	{
		if (Slot->Name == SlotID)
		{
			return Slot;
		}
	}
	return nullptr;
}
