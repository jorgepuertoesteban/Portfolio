// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/JPEIconColorFactory.h"
#include "JPEIconColor.h"

UJPEIconColorFactory::UJPEIconColorFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UJPEIconColor::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UJPEIconColorFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UJPEIconColor* NewAsset = NewObject<UJPEIconColor>(InParent, Class, Name, Flags);
	return NewAsset;
}


