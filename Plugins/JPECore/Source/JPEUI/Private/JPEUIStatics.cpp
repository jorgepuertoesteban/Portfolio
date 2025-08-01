// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "JPEUIStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/PlayerController.h"
#include "UI/CategoryTabWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"

void UJPEUIStatics::CalculatePositionInViewport(APlayerController* PlayerController, float const WidgetSizeX, float const WidgetSizeY, FVector2D& Result)
{
	float MouseScaledX, MouseScaledY;
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(PlayerController, MouseScaledX, MouseScaledY);

	int32 ViewportSizeX, ViewportSizeY;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(PlayerController);

	Result.X = FMath::Clamp(MouseScaledX - WidgetSizeX / 2, 0.f, ViewportSizeX / ViewportScale - WidgetSizeX);
	Result.Y = FMath::Clamp(MouseScaledY - WidgetSizeY / 2, 0.f, ViewportSizeY / ViewportScale - WidgetSizeY);
}