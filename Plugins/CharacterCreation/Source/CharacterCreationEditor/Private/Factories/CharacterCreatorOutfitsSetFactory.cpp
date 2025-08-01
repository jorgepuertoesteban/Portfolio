// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/CharacterCreatorOutfitsSetFactory.h"
#include "CharacterCreatorOutfitsSet.h"

UCharacterCreatorOutfitsSetFactory::UCharacterCreatorOutfitsSetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorOutfitsSet::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCharacterCreatorOutfitsSetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreatorOutfitsSet* NewAsset = NewObject<UCharacterCreatorOutfitsSet>(InParent, Class, Name, Flags);
	return NewAsset;
}