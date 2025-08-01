// Created by Jorge Puerto. All rights reserved.

#include "GameFramework/FPPCharacter.h"
#include "Components/InputComponent.h"

AFPPCharacter::AFPPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called to bind functionality to input
void AFPPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//InputComponent->BindAxis("MoveForward", this, &AFPPCharacter::MoveForward);
	//InputComponent->BindAxis("MoveRight", this, &AFPPCharacter::MoveRight);
}

