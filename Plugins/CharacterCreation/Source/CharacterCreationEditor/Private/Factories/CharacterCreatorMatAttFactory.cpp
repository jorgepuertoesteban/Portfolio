// Created by Jorge Puerto. All Rights Reserved.


#include "Factories/CharacterCreatorMatAttFactory.h"
#include "CharacterCreatorMatAttribute.h"

UUCharacterCreatorMatAttFactory::UUCharacterCreatorMatAttFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorMatAttribute::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UUCharacterCreatorMatAttFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreatorMatAttribute* NewAsset = NewObject<UCharacterCreatorMatAttribute>(InParent, Class, Name, Flags);
	return NewAsset;
}