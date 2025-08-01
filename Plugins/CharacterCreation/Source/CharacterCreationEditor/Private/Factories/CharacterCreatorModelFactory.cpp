// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/CharacterCreatorModelFactory.h"
#include "CharacterCreatorModel.h"

UCharacterCreatorModelFactory::UCharacterCreatorModelFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorModel::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCharacterCreatorModelFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreatorModel* NewAsset = NewObject<UCharacterCreatorModel>(InParent, Class, Name, Flags);
	return NewAsset;
}