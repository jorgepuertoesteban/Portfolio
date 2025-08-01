// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Alerts/RevertAppliedSettingsAlertWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/JPEGameUserSettings.h"

void URevertAppliedSettingsAlertWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RevertButton)
	{
		RevertButton->OnClicked.AddUniqueDynamic(this, &URevertAppliedSettingsAlertWidget::HandleOnRevertButtonClicked);
	}
	if (KeepButton)
	{
		KeepButton->OnClicked.AddUniqueDynamic(this, &URevertAppliedSettingsAlertWidget::HandleOnKeepButtonClicked);
	}

	TimeRemainingText->SetText(FText::AsNumber(CountdownDurationSeconds - 1));
	CountdownEndTimeSeconds = GetWorld()->GetTimeSeconds() + CountdownDurationSeconds;
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &URevertAppliedSettingsAlertWidget::CountdownTick, 1.f);
}

void URevertAppliedSettingsAlertWidget::RevertAppliedSettings()
{
	GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		Settings->RevertAppliedSettings();
	}
	RemoveFromParent();
}

void URevertAppliedSettingsAlertWidget::KeepAppliedSettings()
{
	GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		Settings->KeepAppliedSettings();
	}
	RemoveFromParent();
}

void URevertAppliedSettingsAlertWidget::CountdownTick()
{
	const float RemainingSecondsFloat = CountdownEndTimeSeconds - GetWorld()->GetTimeSeconds();
	const int32 RemainingSecondsInt = FMath::Floor(RemainingSecondsFloat);
	if (RemainingSecondsInt >= 0)
	{
		TimeRemainingText->SetText(FText::AsNumber(RemainingSecondsInt));
		const float TimeTillNextCountdownTick = RemainingSecondsFloat - static_cast<float>(RemainingSecondsInt);
		GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &URevertAppliedSettingsAlertWidget::CountdownTick, TimeTillNextCountdownTick);
	}
	else
	{
		RevertAppliedSettings();
	}
}

void URevertAppliedSettingsAlertWidget::HandleOnRevertButtonClicked()
{
	RevertAppliedSettings();
}

void URevertAppliedSettingsAlertWidget::HandleOnKeepButtonClicked()
{
	KeepAppliedSettings();
}
