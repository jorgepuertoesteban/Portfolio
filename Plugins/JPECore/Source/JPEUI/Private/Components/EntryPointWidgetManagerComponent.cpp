// Created by Jorge Puerto. All Rights Reserved.


#include "Components/EntryPointWidgetManagerComponent.h"
#include "Blueprint/GameViewportSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "GameFramework/HUD.h"
#include "Helpers/InputModeHelper.h"
#include "Interfaces/InputModePawnInterface.h"
#include "Logs/JPEUILog.h"

class UGameViewportSubsystem;

void UEntryPointWidgetManagerComponent::OnRegister()
{
	Super::OnRegister();

	if (UGameViewportSubsystem* ViewportSubsystem = UGameViewportSubsystem::Get(GetWorld()))
	{
		ViewportSubsystem->OnWidgetAdded.AddWeakLambda(
			this, [this](UWidget* NewWidget, const ULocalPlayer* WidgetLocalPlayer) //WidgetLocalPlayer is nullptr most of the time, so we can't rely on it
			{
				if (!NewWidget)
				{
					UE_LOG(JPEUILog, Error, TEXT("UEntryPointWidgetManagerComponent::BeginPlay::OnWidgetAdded NewWidget is nullptr"))
					return;
				}
				const APlayerController* PlayerController = GetOwningPlayerController();
				if (!PlayerController)
				{
					UE_LOG(JPEUILog, Error, TEXT("UEntryPointWidgetManagerComponent::BeginPlay::OnWidgetAdded No PlayerController Found"))
					return;
				}
				if (NewWidget->GetOwningPlayer() != PlayerController)
				{
					return;
				}
				if (EntryPointWidget == NewWidget)
				{
					return;
				}
				if (!EntryClasses.Contains(NewWidget->GetClass()))
				{
					return;
				}
				if (EntryPointWidget)
				{
					if (EntryClassesToStack.Contains(EntryPointWidget->GetClass()))
					{
						EntryPointWidgetStack.Push(EntryPointWidget);
					}
					EntryPointWidget->RemoveFromParent();
				}
				EntryPointWidget = NewWidget;
				UInputModeHelper::SetInputUIOnly(GetOwningPlayerController());
			}
		);
		ViewportSubsystem->OnWidgetRemoved.AddWeakLambda(
			this, [this](const UWidget* WidgetRemoved)
			{
				if (!EntryPointWidget || WidgetRemoved != EntryPointWidget || EntryPointWidgetStack.Contains(WidgetRemoved) || GetWorld()->bIsTearingDown)
				{
					return;
				}

				EntryPointWidget = EntryPointWidgetStack.IsEmpty() ? nullptr : EntryPointWidgetStack.Pop();
				if (UUserWidget* EntryPointUserWidget = Cast<UUserWidget>(EntryPointWidget))
				{
					EntryPointUserWidget->AddToViewport();
				}

				if (const APawn* OwningPawn = GetOwningPawn(); OwningPawn && OwningPawn->Implements<UInputModePawnInterface>())
				{
					UInputModeHelper::SetInputMode(GetOwningPlayerController(), IInputModePawnInterface::Execute_GetInputMode(OwningPawn));
				}
			}
		);
	}
}

void UEntryPointWidgetManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = GetOwningPlayerController())
	{
		PlayerController->OnPossessedPawnChanged.AddDynamic(this, &UEntryPointWidgetManagerComponent::OnPossessedPawnChanged);
		if (EntryPointWidget) //There is an EntryPointWidget on top, so we don't override the input, we return
		{
			return;
		}
		UInputModeHelper::SetInputModeFromPawn(PlayerController, GetOwningPawn());
	}
}

APlayerController* UEntryPointWidgetManagerComponent::GetOwningPlayerController() const
{
	const AHUD* MyHUD = Cast<AHUD>(GetOwner());
	return MyHUD ? MyHUD->GetOwningPlayerController() : nullptr;
}

APawn* UEntryPointWidgetManagerComponent::GetOwningPawn() const
{
	const APlayerController* PC = GetOwningPlayerController();
	return PC ? PC->GetPawn() : nullptr;
}

void UEntryPointWidgetManagerComponent::ResetDesiredInput() const
{
	if (EntryPointWidget) //There is an EntryPointWidget on top we reset the desired input again
	{
		UInputModeHelper::SetInputUIOnly(GetOwningPlayerController());
	}
	else
	{
		UInputModeHelper::SetInputModeFromPawn(GetOwningPlayerController(), GetOwningPawn());
	}
}

UEntryPointWidgetManagerComponent* UEntryPointWidgetManagerComponent::GetEntryPointWidgetManagerComponent(const AActor* Owner)
{
	if (!Owner)
	{
		return nullptr;
	}
	return Owner->GetComponentByClass<UEntryPointWidgetManagerComponent>();
}

void UEntryPointWidgetManagerComponent::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (EntryPointWidget) //There is an EntryPointWidget on top, so we don't override the input, we return
	{
		return;
	}
	if (!NewPawn)
	{
		return;
	}
	UInputModeHelper::SetInputModeFromPawn(GetOwningPlayerController(), NewPawn);
}
