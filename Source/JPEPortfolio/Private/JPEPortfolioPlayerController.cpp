// Copyright Epic Games, Inc. All Rights Reserved.


#include "JPEPortfolioPlayerController.h"
#include "EnhancedInputSubsystems.h"
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
}
