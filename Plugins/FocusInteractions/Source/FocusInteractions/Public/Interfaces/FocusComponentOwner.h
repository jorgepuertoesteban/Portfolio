// Created by Bionic Ape. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FocusComponentOwner.generated.h"

class UFocusComponent;

UINTERFACE(Blueprintable)
class UFocusComponentOwner : public UInterface
{
	GENERATED_BODY()
};

class FOCUSINTERACTIONS_API IFocusComponentOwner
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UFocusComponent* GetFocusComponent() const;

	virtual UFocusComponent* GetFocusComponent_Implementation() const { return nullptr; };

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetFocusComponent(UFocusComponent* FocusComponent);

	virtual void SetFocusComponent_Implementation(UFocusComponent* FocusComponent) {};
};
