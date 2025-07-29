// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionSequenceComponentOwner.generated.h"

class UActionSequenceComponent;

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UActionSequenceComponentOwner : public UInterface
{
	GENERATED_BODY()
};

class FOCUSINTERACTIONS_API IActionSequenceComponentOwner
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UActionSequenceComponent* GetActionSequenceComponent();

	virtual UActionSequenceComponent* GetActionSequenceComponent_Implementation() { return nullptr; };

};
