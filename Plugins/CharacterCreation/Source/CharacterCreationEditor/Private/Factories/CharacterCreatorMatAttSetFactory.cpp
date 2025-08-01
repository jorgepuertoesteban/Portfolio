// Created by Jorge Puerto. All Rights Reserved.


#include "Factories/CharacterCreatorMatAttSetFactory.h"
#include "CharacterCreatorMatAttributeSet.h"

UCharacterCreatorMatAttSetFactory::UCharacterCreatorMatAttSetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorMatAttributesSet::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCharacterCreatorMatAttSetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreatorMatAttributesSet* NewAsset = NewObject<UCharacterCreatorMatAttributesSet>(InParent, Class, Name, Flags);
	return NewAsset;
}