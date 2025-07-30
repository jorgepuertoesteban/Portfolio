// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFramework/JPEPortfolioCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/HUD.h"

#include "Interfaces/FocusTracerHUDInterface.h"
#include "Components/FocusableComponent.h"
#include "Components/DecalComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AJPEPortfolioCharacter::AJPEPortfolioCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	FocusedDecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("FocusedDecalComp"));
	FocusedDecalComp->SetupAttachment(GetMesh());
	FocusedDecalComp->SetVisibility(false);

	FocusableComp = CreateDefaultSubobject<UFocusableComponent>(TEXT("FocusableComp"));
	FocusableComp->OnStartFocusDelegate.AddDynamic(this, &AJPEPortfolioCharacter::OnStartFocus);
	FocusableComp->OnEndFocusDelegate.AddDynamic(this, &AJPEPortfolioCharacter::OnEndFocus);

}

void AJPEPortfolioCharacter::Restart()
{
	Super::Restart();

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PC->IsLocalController())
	{
		if (IFocusTracerHUDInterface* FocusTracerHUD = Cast<IFocusTracerHUDInterface>(PC->GetHUD()))
		{
			FocusTracerHUD->ShowFocusTraceInterface();
		}
	}
}

void AJPEPortfolioCharacter::PossessedBy(AController* NewController)
{
	CameraBoom->SetComponentTickEnabled(true);
	Super::PossessedBy(NewController);
}


void AJPEPortfolioCharacter::UnPossessed()
{
	CameraBoom->SetComponentTickEnabled(false);
	Super::UnPossessed();
}


void AJPEPortfolioCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJPEPortfolioCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AJPEPortfolioCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AJPEPortfolioCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AJPEPortfolioCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AJPEPortfolioCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AJPEPortfolioCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AJPEPortfolioCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AJPEPortfolioCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AJPEPortfolioCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AJPEPortfolioCharacter::OnStartFocus(const FFocusInfo FocusTraceInfo)
{
	FocusedDecalComp->SetVisibility(true);
}

void AJPEPortfolioCharacter::OnEndFocus()
{
	FocusedDecalComp->SetVisibility(false);
}