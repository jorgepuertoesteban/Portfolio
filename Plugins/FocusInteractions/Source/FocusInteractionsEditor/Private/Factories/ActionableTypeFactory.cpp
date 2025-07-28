// Created by Bionic Ape. All Rights Reserved.

#include "Factories/ActionableTypeFactory.h"
#include "ActionableType.h"

UActionableTypeFactory::UActionableTypeFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UActionableType::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UActionableTypeFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UActionableType* NewAsset = NewObject<UActionableType>(InParent, Class, Name, Flags);
	return NewAsset;
}


