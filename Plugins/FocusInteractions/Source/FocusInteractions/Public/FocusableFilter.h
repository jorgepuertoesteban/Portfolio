// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FocusableFilter.generated.h"

class UFocusableComponent;
class UFocusComponent;
class AActor;

/**
 *
 */
UCLASS(Abstract)
class FOCUSINTERACTIONS_API UFocusableFilter : public UObject
{
	GENERATED_BODY()

public:

	virtual bool IsAllowed(const UFocusComponent* FocusComponent, const AActor* FocusableActor, const UFocusableComponent* FocusableComponent) const { return true; }
};

