// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "PointAndClickCamera.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class FOCUSINTERACTIONS_API APointAndClickCamera : public ACameraActor
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "PointAndClick")
	UBoxComponent* ShapeCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "PointAndClick")
	UBoxComponent* InitializationDetectionCollision;

	UPROPERTY(EditAnywhere)
	bool bAbsoluteBoxRotation = true;

public:
	
	APointAndClickCamera();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;
};
