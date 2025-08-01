// Created by Jorge Puerto. All Rights Reserved.


#include "JPEUISubsystem.h"
#include "GameFramework/PlayerController.h"
#include "JPEUIConfig.h"
#include "Blueprint/UserWidget.h"
#include "Engine/GameInstance.h"
#include "UI/AlertWidget.h"
#include "UI/ConfirmWidget.h"
#include "UObject/ObjectPtr.h"

UJPEUISubsystem* UJPEUISubsystem::MyInstance;


bool UJPEUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UGameInstance* GameInstance = Cast<UGameInstance>(Outer);
	return GameInstance && !GameInstance->IsDedicatedServerInstance();
}

void UJPEUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	MyInstance = this;

	if (!UIConfigProxy.IsNull())
	{
		UIConfig = UIConfigProxy.LoadSynchronous();
	}

	if (bShouldDisableWorldRendering)
	{
		FCoreUObjectDelegates::PostLoadMapWithWorld.AddWeakLambda(this, [this](UWorld* World)
		{
			// if (UGameViewportClient* Viewport = World->GetGameViewport())
			// {
			// 	Viewport->bDisableWorldRendering = true;
			// }
		});
	}
}

template <typename OwnerType /*= UObject*/>
UAlertWidget* UJPEUISubsystem::CreateAlertWidget(OwnerType* Owner, const FText& Text, const int32 ZOrder)
{
	UAlertWidget* AlertWidget = CreateWidget<UAlertWidget>(Owner, UJPEUISubsystem::GetInstance()->GetUIConfig()->SuccessAlertWidgetClass);
	AlertWidget->SetBodyText(Text);
	AlertWidget->AddToViewport(ZOrder);
	return AlertWidget;
}

UAlertWidget* UJPEUISubsystem::CreateAlert(UUserWidget* Owner, const FText& Text, const int32 ZOrder)
{
	return CreateAlertWidget(Owner, Text, ZOrder);
}


UAlertWidget* UJPEUISubsystem::CreateAlert(APlayerController* Owner, const FText& Text, const int32 ZOrder)
{
	return CreateAlertWidget(Owner, Text, ZOrder);
}

template <typename OwnerType /*= UObject*/>
UConfirmWidget* UJPEUISubsystem::CreateConfirmWidget(OwnerType* Owner, const FText& Text, const int32 ZOrder)
{
	UConfirmWidget* ConfirmWidget = CreateWidget<UConfirmWidget>(Owner, UJPEUISubsystem::GetInstance()->GetUIConfig()->ConfirmWidgetClass);
	ConfirmWidget->SetBodyText(Text);
	ConfirmWidget->AddToViewport(ZOrder);
	return ConfirmWidget;
}

UConfirmWidget* UJPEUISubsystem::CreateConfirm(UUserWidget* Owner, const FText& Text, const int32 ZOrder)
{
	return CreateConfirmWidget(Owner, Text, ZOrder);
}

UConfirmWidget* UJPEUISubsystem::CreateConfirm(APlayerController* Owner, const FText& Text, const int32 ZOrder)
{
	return CreateConfirmWidget(Owner, Text, ZOrder);
}

UJPEUIConfig* UJPEUISubsystem::GetUIConfig()
{
	if (UIConfig == nullptr)
	{
		UIConfig = UIConfigProxy.LoadSynchronous();
	}
	return UIConfig;
}
