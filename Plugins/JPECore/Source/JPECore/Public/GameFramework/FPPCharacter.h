// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPPCharacter.generated.h"

UCLASS()
class JPECORE_API AFPPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFPPCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
