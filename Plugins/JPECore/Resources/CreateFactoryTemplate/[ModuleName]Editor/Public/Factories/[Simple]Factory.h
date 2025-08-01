// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "[Factory]Factory.generated.h"

/**
 *
 */
UCLASS()
class [Module_Upper]EDITOR_API U[Factory]Factory : public UFactory
{
	GENERATED_BODY()


	U[Factory]Factory(const FObjectInitializer& ObjectInitializer);

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override { return true; }
	// End of UFactory interface

};
