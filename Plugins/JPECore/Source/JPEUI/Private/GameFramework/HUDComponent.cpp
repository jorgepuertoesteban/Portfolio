// Created by Jorge Puerto. All Rights Reserved.


#include "Components/HUDComponent.h"

#include "Blueprint/UserWidget.h"
#include "Interfaces/JPEHUDInterface.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/HUD.h"


const FInputModeGameAndUI UHUDComponent::InputModeGameAndUI = FInputModeGameAndUI().SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock).SetHideCursorDuringCapture(true);
const FInputModeGameOnly UHUDComponent::InputModeGameOnly = FInputModeGameOnly();
const FInputModeUIOnly UHUDComponent::InputModeUIOnly = FInputModeUIOnly();

APlayerController* UHUDComponent::GetController() const
{
	return Cast<APlayerController>(GetHUD()->GetOwningPlayerController());
}

APawn* UHUDComponent::GetControlledPawn() const
{
	if (const APlayerController* PC = GetController())
	{
		return PC->GetPawn();
	}
	return nullptr;
}

AHUD* UHUDComponent::GetHUD() const
{
	return Cast<AHUD>(GetOwner());
}

ULocalPlayer* UHUDComponent::GetOwningLocalPlayer() const
{
	if (const APlayerController* PC = GetController())
	{
		return PC->GetLocalPlayer();
	}
	return nullptr;
}

TScriptInterface<IJPEHUDInterface> UHUDComponent::GetJPEHUD() const
{
	return GetHUD();
}

UUserWidget* UHUDComponent::NewWidget(const TSubclassOf<UUserWidget> WidgetClass) const
{
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UHUDComponent::NewWidget(const TSubclassOf<UUserWidget>): Widget Class is not valid."));
		return nullptr;
	}
	APlayerController* PC = GetController();
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UHUDComponent::NewWidget(const TSubclassOf<UUserWidget>): PlayerController is not valid."));
		return nullptr;
	}
	return CreateWidget<UUserWidget>(PC, WidgetClass);
}

void UHUDComponent::ShowWidget(UUserWidget* Widget)
{
	if (!Widget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UHUDComponent::ShowWidget(UUserWidget*): Widget is not valid."));
		return;
	}
	if (Widget->IsInViewport())
	{
		UE_LOG(LogTemp, Warning, TEXT("UHUDComponent::ShowWidget(UUserWidget*): Widget is already visible."));
		return;
	}
	
	Widget->AddToViewport();
}

void UHUDComponent::HideWidget(UUserWidget* Widget)
{
	if (!Widget || !Widget->IsInViewport())
	{
		UE_LOG(LogTemp, Warning, TEXT("UHUDComponent::HideWidget(UUserWidget*): Widget is already hidden."));
		return;
	}
	
	Widget->RemoveFromParent();
}

void UHUDComponent::SetInputModeGameAndUI()
{
	if (APlayerController* PC = GetController())
	{
		PC->SetInputMode(InputModeGameAndUI);
		PC->bShowMouseCursor = true;
	}
}

void UHUDComponent::SetInputModeGameOnly()
{
	if (APlayerController* PC = GetController())
	{
		PC->SetInputMode(InputModeGameOnly);
		PC->bShowMouseCursor = false;
	}
}

void UHUDComponent::SetInputModeUIOnly()
{
	if (APlayerController* PC = GetController())
	{
		PC->SetInputMode(InputModeUIOnly);
		PC->bShowMouseCursor = true;
	}
}