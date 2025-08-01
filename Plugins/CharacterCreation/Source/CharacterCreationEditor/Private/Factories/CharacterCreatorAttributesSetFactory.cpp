// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/CharacterCreatorAttributesSetFactory.h"
#include "CharacterCreatorAttributesSet.h"

UCharacterCreatorAttributesSetFactory::UCharacterCreatorAttributesSetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorAttributesSet::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCharacterCreatorAttributesSetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreatorAttributesSet* NewAsset = NewObject<UCharacterCreatorAttributesSet>(InParent, Class, Name, Flags);
	return NewAsset;
}