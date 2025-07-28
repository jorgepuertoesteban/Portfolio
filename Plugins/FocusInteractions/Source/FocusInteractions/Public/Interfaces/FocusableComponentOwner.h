// Created by Bionic Ape. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FocusableComponentOwner.generated.h"

class UFocusableComponent;

UINTERFACE(Blueprintable)
class UFocusableComponentOwner : public UInterface
{
	GENERATED_BODY()
};

class FOCUSINTERACTIONS_API IFocusableComponentOwner
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UFocusableComponent* GetFocusableComponent();

	virtual UFocusableComponent* GetFocusableComponent_Implementation() { return nullptr; };

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetFocusableComponent(UFocusableComponent* FocusableComponent);

	virtual void SetFocusableComponent_Implementation(UFocusableComponent* FocusableComponent) {};
};
