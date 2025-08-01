// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "JPEColorPaletteFactory.generated.h"

/**
*
*/
UCLASS()
class JPEUIEDITOR_API UJPEColorPaletteFactory : public UFactory
{
	GENERATED_BODY()


	UJPEColorPaletteFactory(const FObjectInitializer& ObjectInitializer);

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override { return true; }
	// End of UFactory interface

};
