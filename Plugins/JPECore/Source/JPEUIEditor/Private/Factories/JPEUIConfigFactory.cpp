// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/JPEUIConfigFactory.h"
#include "JPEUIConfig.h"

UJPEUIConfigFactory::UJPEUIConfigFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UJPEUIConfig::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UJPEUIConfigFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UJPEUIConfig* NewAsset = NewObject<UJPEUIConfig>(InParent, Class, Name, Flags);
	return NewAsset;
}


