// Created by Jorge Puerto. All Rights Reserved.


#include "Components/ControlComponent.h"

#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/JPEHUDInterface.h"


APlayerController* UControlComponent::GetController() const
{
	return Cast<APlayerController>(GetOwner());
}

APawn* UControlComponent::GetControlledPawn() const
{
	if (const APlayerController* PC = GetController())
	{
		return PC->GetPawn();
	}
	return nullptr;
}

AHUD* UControlComponent::GetHUD() const
{
	if (const APlayerController* PC = GetController())
	{
		return PC->GetHUD();
	}
	return nullptr;
}

TScriptInterface<IJPEHUDInterface> UControlComponent::GetJPEHUD() const
{
	return GetHUD();
}

void UControlComponent::Client_Notify_Implementation(bool bIsSuccessful, const FString& Message)
{
	TScriptInterface<IJPEHUDInterface> JPEHUD = GetJPEHUD();
	if (JPEHUD)
	{
		JPEHUD->NotifyResponse(bIsSuccessful, FText::FromString(Message));
	}
}