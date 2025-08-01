// Created by Jorge Puerto. All Rights Reserved.


#include "Factories/CharacterCreatorVectorMatAttFactory.h"
#include "CharacterCreatorVectorMatAttribute.h"

UCharacterCreatorVectorMatAttFactory::UCharacterCreatorVectorMatAttFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorVectorMatAttribute::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCharacterCreatorVectorMatAttFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreatorVectorMatAttribute* NewAsset = NewObject<UCharacterCreatorVectorMatAttribute>(InParent, Class, Name, Flags);
	return NewAsset;
}