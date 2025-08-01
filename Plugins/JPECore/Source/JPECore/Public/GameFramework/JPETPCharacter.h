// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JPETPCharacter.generated.h"

class APlayerCameraManager;

UCLASS()
class JPECORE_API AJPETPCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(Transient)
	APlayerCameraManager* CameraManagerCache;

public:
	// Sets default values for this character's properties
	AJPETPCharacter();

	virtual void Restart() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MoveForward(float Val);
	virtual void MoveRight(float Val);
};
