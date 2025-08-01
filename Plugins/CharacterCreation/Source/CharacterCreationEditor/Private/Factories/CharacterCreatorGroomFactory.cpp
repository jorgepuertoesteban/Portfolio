// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/CharacterCreatorGroomFactory.h"
#include "CharacterCreatorGroom.h"

UCharacterCreatorGroomFactory::UCharacterCreatorGroomFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorGroom::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCharacterCreatorGroomFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreatorGroom* NewAsset = NewObject<UCharacterCreatorGroom>(InParent, Class, Name, Flags);
	return NewAsset;
}