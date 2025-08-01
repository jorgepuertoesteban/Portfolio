// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/CharacterCreatorColorsSetFactory.h"
#include "CharacterCreatorColorsSet.h"

UCharacterCreatorColorsSetFactory::UCharacterCreatorColorsSetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorColorsSet::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCharacterCreatorColorsSetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreatorColorsSet* NewAsset = NewObject<UCharacterCreatorColorsSet>(InParent, Class, Name, Flags);
	return NewAsset;
}