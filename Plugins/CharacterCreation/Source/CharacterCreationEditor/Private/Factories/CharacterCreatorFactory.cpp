// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/CharacterCreatorFactory.h"
#include "CharacterCreator.h"

UCharacterCreatorFactory::UCharacterCreatorFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreator::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCharacterCreatorFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreator* NewAsset = NewObject<UCharacterCreator>(InParent, Class, Name, Flags);
	return NewAsset;
}