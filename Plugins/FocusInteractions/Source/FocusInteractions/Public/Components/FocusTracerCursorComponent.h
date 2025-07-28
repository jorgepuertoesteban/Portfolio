// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/FocusTracerComponent.h"
#include "FocusTracerCursorComponent.generated.h"

class APlayerController;
class APawn;

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class FOCUSINTERACTIONS_API UFocusTracerCursorComponent : public UFocusTracerComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient)
	APlayerController* MyPlayerController;

public:

	UFocusTracerCursorComponent();

	virtual void GetStartLocationAndForwardVector(FVector& StartTrace, FVector& RotVector) override;

	bool TrySetController();
};
