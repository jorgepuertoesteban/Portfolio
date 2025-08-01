// Created by Jorge Puerto. All Rights Reserved.


#include "Factories/CharacterCreatorTextureMatAttFactory.h"
#include "CharacterCreatorTextureMatAttribute.h"

UCharacterCreatorTextureMatAttFactory::UCharacterCreatorTextureMatAttFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorTextureMatAttribute::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCharacterCreatorTextureMatAttFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreatorTextureMatAttribute* NewAsset = NewObject<UCharacterCreatorTextureMatAttribute>(InParent, Class, Name, Flags);
	return NewAsset;
}