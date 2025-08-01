// Created by Jorge Puerto. All Rights Reserved.

#include "Factories/JPEColorPaletteFactory.h"
#include "JPEColorPalette.h"

UJPEColorPaletteFactory::UJPEColorPaletteFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SupportedClass = UJPEColorPalette::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UJPEColorPaletteFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
	UJPEColorPalette* NewAsset = NewObject<UJPEColorPalette>(InParent, Class, Name, Flags);
	return NewAsset;
}


