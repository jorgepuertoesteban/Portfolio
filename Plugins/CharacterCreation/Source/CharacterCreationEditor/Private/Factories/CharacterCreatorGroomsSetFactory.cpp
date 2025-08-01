// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/CharacterCreatorGroomsSetFactory.h"
#include "CharacterCreatorGroomsSet.h"

UCharacterCreatorGroomsSetFactory::UCharacterCreatorGroomsSetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UCharacterCreatorGroomsSet::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCharacterCreatorGroomsSetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UCharacterCreatorGroomsSet* NewAsset = NewObject<UCharacterCreatorGroomsSet>(InParent, Class, Name, Flags);
	return NewAsset;
}