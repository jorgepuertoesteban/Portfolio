// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "TransitionCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class JPECORE_API ATransitionCameraActor : public ACameraActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Key;

protected:

	virtual void BeginPlay() override;
	
};
