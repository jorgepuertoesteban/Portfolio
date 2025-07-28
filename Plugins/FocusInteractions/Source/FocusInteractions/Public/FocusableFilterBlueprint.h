// Created by Bionic Ape. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FocusableFilter.h"
#include "FocusableFilterBlueprint.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class FOCUSINTERACTIONS_API UFocusableFilterBlueprint : public UFocusableFilter
{
	GENERATED_BODY()
	
public:

	bool IsAllowed(const UFocusComponent* FocusComponent, const AActor* FocusableActor, const UFocusableComponent* FocusableComponent) const override { return IsAllowedBP(FocusComponent, FocusableActor, FocusableComponent); }

	UFUNCTION(BlueprintImplementableEvent, Category = "FocusableFilter")
	bool IsAllowedBP(const UFocusComponent* FocusComponent, const AActor* FocusableActor, const UFocusableComponent* FocusableComponent) const;
};
