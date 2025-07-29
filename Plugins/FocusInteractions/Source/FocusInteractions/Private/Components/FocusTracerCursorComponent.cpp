// Created by Jorge Puerto. All Rights Reserved.

#include "Components/FocusTracerCursorComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UFocusTracerCursorComponent::UFocusTracerCursorComponent() :Super()
{
	bUseLineTrace = true;
	bForceCloseDistance = true;
}

void UFocusTracerCursorComponent::GetStartLocationAndForwardVector(FVector& StartTrace, FVector& RotVector)
{
	if (MyPlayerController || TrySetController())
	{
		FVector2D MousePosition;
		MyPlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
		UGameplayStatics::DeprojectScreenToWorld(MyPlayerController, MousePosition, StartTrace, RotVector);
	}
}

bool UFocusTracerCursorComponent::TrySetController()
{
	if (AController* MyController = GetController())
	{
		MyPlayerController = Cast<APlayerController>(MyController);
	}
	return MyPlayerController != nullptr;
}
