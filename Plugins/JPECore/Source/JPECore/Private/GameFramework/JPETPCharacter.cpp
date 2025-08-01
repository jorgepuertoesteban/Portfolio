// Created by Jorge Puerto. All Rights Reserved.


#include "GameFramework/JPETPCharacter.h"

#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AJPETPCharacter::AJPETPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void AJPETPCharacter::Restart()
{
	Super::Restart();

	const AController* MyController = GetController();

	if (MyController && MyController->IsPlayerController() && MyController->IsLocalController())
	{
		const APlayerController* PC = Cast<APlayerController>(GetController());
		CameraManagerCache = PC->PlayerCameraManager;
	}
	else
	{
		CameraManagerCache = nullptr;
	}
}

void AJPETPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//InputComponent->BindAction("Action1", IE_Pressed, this, &AMetaverseCharacter::Action1Pressed);
	//InputComponent->BindAction("Action2", IE_Pressed, this, &AMetaverseCharacter::Action2Pressed);
	//InputComponent->BindAction("Action3", IE_Pressed, this, &AMetaverseCharacter::Action3Pressed);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAxis("MoveForward", this, &AJPETPCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AJPETPCharacter::MoveRight);
}
void AJPETPCharacter::MoveForward(float Value)
{
	if (Value != 0.0f && (CameraManagerCache || GetController()))
	{
		//UE_LOG(LogTemp, Log, TEXT("My role is %s"), *UEnum::GetValueAsString(GetLocalRole()));
		//find out which way is forward
		FRotator const Rotation = CameraManagerCache ? CameraManagerCache->GetCameraRotation() : GetController()->GetControlRotation();
		FRotator const YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		FVector const Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AJPETPCharacter::MoveRight(float Value)
{
	if (Value != 0.0f && (CameraManagerCache || GetController()))
	{
		//UE_LOG(LogTemp, Log, TEXT("My role is %s"), *UEnum::GetValueAsString(GetLocalRole()));
		//find out which way is forward
		FRotator const Rotation = CameraManagerCache ? CameraManagerCache->GetCameraRotation() : GetController()->GetControlRotation();
		FRotator const YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		FVector const Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}