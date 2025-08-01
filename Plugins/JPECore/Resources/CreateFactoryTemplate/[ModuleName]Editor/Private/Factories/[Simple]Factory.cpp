// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/[Factory]Factory.h"
#include "Model/[Factory].h"

U[Factory]Factory::U[Factory]Factory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = U[Factory]::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* U[Factory]Factory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	U[Factory]* NewAsset = NewObject<U[Factory]>(InParent, Class, Name, Flags);
	return NewAsset;
}
