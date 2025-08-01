// Created by Jorge Puerto. All Rights Reserved.


#include "GameFramework/JPEHUD.h"
#include "JPEUISubsystem.h"
#include "Helpers/InputModeHelper.h"

const FInputModeGameAndUI AJPEHUD::InputModeGameAndUI = FInputModeGameAndUI().SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock).SetHideCursorDuringCapture(false);
const FInputModeGameOnly AJPEHUD::InputModeGameOnly = FInputModeGameOnly();
const FInputModeUIOnly AJPEHUD::InputModeUIOnly = FInputModeUIOnly();

#define LOCTEXT_NAMESPACE "JPEHUD"

UAlertWidget* AJPEHUD::CreateAlert(const FText& Message)
{
	UAlertWidget* AlertWidget = UJPEUISubsystem::GetInstance()->CreateAlertWidget(GetOwningPlayerController(), Message);
	return AlertWidget;
}

UAlertWidget* AJPEHUD::NotifyResponse(bool bIsSuccessful, const FText& Message)
{
	return CreateAlert(bIsSuccessful ? LOCTEXT("NotifyResponse", "Successful") : Message);
}

UConfirmWidget* AJPEHUD::CreateConfirm(const FText& Message)
{
	UE_LOG(LogTemp, Log, TEXT("AJPEHUD::CreateConfirm"));
	UConfirmWidget* AlertWidget = UJPEUISubsystem::GetInstance()->CreateConfirmWidget(GetOwningPlayerController(), Message);
	return AlertWidget;
}


#undef LOCTEXT_NAMESPACE // "JPEHUD"

