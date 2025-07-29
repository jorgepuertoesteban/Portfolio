// Created by Jorge Puerto. All Rights Reserved.


#include "Components/PointAndClickHUDComponent.h"
#include "UI/PointAndClickMenuWidget.h"
#include "GameFramework/HUD.h"


class UEnhancedInputLocalPlayerSubsystem;

UPointAndClickHUDComponent* UPointAndClickHUDComponent::GetPointAndClickHUDComponent(const AHUD* HUD)
{
	return HUD ? HUD->FindComponentByClass<UPointAndClickHUDComponent>() : nullptr;
}

APlayerController* UPointAndClickHUDComponent::GetOwningPlayerController() const
{
	const AHUD* HUD = Cast<AHUD>(GetOwner());
	if (!HUD)
	{
		return nullptr;
	}
	return HUD->GetOwningPlayerController();
}

void UPointAndClickHUDComponent::ClearWidgets()
{
	if (PointAndClickMenuWidget)
	{
		PointAndClickMenuWidget->RemoveFromParent();
		PointAndClickMenuWidget = nullptr;
	}
}

void UPointAndClickHUDComponent::ShowInGameWidget()
{
	if (!PointAndClickMenuWidget)
	{
		PointAndClickMenuWidget = CreateWidget<UPointAndClickMenuWidget>(GetOwningPlayerController(), PointAndClickMenuWidgetClass);
	}
	if (PointAndClickMenuWidget)
	{
		PointAndClickMenuWidget->AddToViewport(1);
	}
}


void UPointAndClickHUDComponent::SelectAction(const int32 ActionIndex) const
{
	if (PointAndClickMenuWidget)
	{
		PointAndClickMenuWidget->SelectIndexAction(ActionIndex);
	}
	FSlateApplication::Get().QueryCursor();
}
