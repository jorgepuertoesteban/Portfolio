// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/FocusTracerComponent.h"
#include "FocusTracerPawnComponent.generated.h"

class AController;
class APawn;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FOCUSINTERACTIONS_API UFocusTracerPawnComponent : public UFocusTracerComponent
{
	GENERATED_BODY()

protected:

	void BeginPlay() override;

public:

	virtual APawn* GetActionPawn() const override { return OwnerPawn; }

	virtual AController* GetActionController() const override { return OwnerPawn->GetController(); }
};
