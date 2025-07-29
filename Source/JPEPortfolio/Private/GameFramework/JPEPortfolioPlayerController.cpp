// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameFramework/JPEPortfolioPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Components/FocusTracerComponent.h"

AJPEPortfolioPlayerController::AJPEPortfolioPlayerController()
{
	FocusTracerComponent = CreateDefaultSubobject<UFocusTracerComponent>(TEXT("FocusTracerComponent"));
}

void AJPEPortfolioPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Contexts
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(FocusAction1, ETriggerEvent::Triggered, this, &AJPEPortfolioPlayerController::Action1Pressed);
		EnhancedInputComponent->BindAction(FocusAction2, ETriggerEvent::Triggered, this, &AJPEPortfolioPlayerController::Action2Pressed);
		EnhancedInputComponent->BindAction(FocusAction3, ETriggerEvent::Triggered, this, &AJPEPortfolioPlayerController::Action3Pressed);
	}
}

void AJPEPortfolioPlayerController::Action1Pressed()
{
	FocusTracerComponent->PerformAction(0);
}

void AJPEPortfolioPlayerController::Action2Pressed()
{
	FocusTracerComponent->PerformAction(1);
}

void AJPEPortfolioPlayerController::Action3Pressed()
{
	FocusTracerComponent->PerformAction(2);
}
