// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/CharacterCreatorAttributeFactory.h"
#include "CharacterCreatorAttribute.h"

UCharacterCreatorAttributeFactory::UCharacterCreatorAttributeFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorAttribute::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCharacterCreatorAttributeFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreatorAttribute* NewAsset = NewObject<UCharacterCreatorAttribute>(InParent, Class, Name, Flags);
	return NewAsset;
}