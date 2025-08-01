// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/CharacterCreatorOutfitSlotFactory.h"
#include "CharacterCreatorOutfitSlot.h"

UCharacterCreatorOutfitSlotFactory::UCharacterCreatorOutfitSlotFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorOutfitSlot::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCharacterCreatorOutfitSlotFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreatorOutfitSlot* NewAsset = NewObject<UCharacterCreatorOutfitSlot>(InParent, Class, Name, Flags);
	return NewAsset;
}