// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Items/Common/SettingsInputKeySelectorWidget.h"

#include "GameFramework/JPEGameUserSettings.h"
#include "Common/AdvancedInputKeySelector.h"
#include "Components/TextBlock.h"
#include "Settings/Alerts/DuplicatedKeyAlertWidget.h"

bool USettingsInputKeySelectorWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (!NameLabel) return false;

		if (bUseEnhancedInputSystem)
		{
			NameLabel->SetText(UJPEInputUtils::GetDisplayNameForActionEnhanced(ActionEnhanced));
		}
		else
		{
			NameLabel->SetText(UJPEInputUtils::GetDisplayNameForActionTraditional(ActionTraditional));
		}
		
		return true;
	}
	return false;
}

void USettingsInputKeySelectorWidget::UpdateUIValues()
{
	const auto [Main, Alt] = GetCurrentSettingValue();
	if (InputKeySelector)
	{
		InputKeySelector->SetSelectedKey(Main);
		InputKeySelector->OnKeySelected.AddUniqueDynamic(this, &USettingsInputKeySelectorWidget::HandleOnInputKeySelected);
		InputKeySelector->OnIsSelectingKeyChanged.AddUniqueDynamic(this, &USettingsInputKeySelectorWidget::HandleOnIsSelectingKeyChanged);
	}
	if (InputKeySelectorAlt)
	{
		InputKeySelectorAlt->SetSelectedKey(Alt);
		InputKeySelectorAlt->OnKeySelected.AddUniqueDynamic(this, &USettingsInputKeySelectorWidget::HandleOnAltInputKeySelected);
		InputKeySelectorAlt->OnIsSelectingKeyChanged.AddUniqueDynamic(this, &USettingsInputKeySelectorWidget::HandleOnAltIsSelectingKeyChanged);
	}
}

void USettingsInputKeySelectorWidget::HandleOnInputKeySelected(const FInputChord& NewValue)
{
	HandleNewKeyBinding(NewValue, true);
}

void USettingsInputKeySelectorWidget::HandleOnAltInputKeySelected(const FInputChord& NewValue)
{
	HandleNewKeyBinding(NewValue, false);
}

void USettingsInputKeySelectorWidget::HandleOnIsSelectingKeyChanged(const bool bNewIsSelectingKey)
{
	if (InputKeySelectorAlt)
	{
		InputKeySelectorAlt->SetVisibility(bNewIsSelectingKey ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
}

void USettingsInputKeySelectorWidget::HandleOnAltIsSelectingKeyChanged(const bool bNewIsSelectingKey)
{
	if (InputKeySelector)
	{
		InputKeySelector->SetVisibility(bNewIsSelectingKey ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
}

FDualInputChord USettingsInputKeySelectorWidget::GetCurrentSettingValue() const
{
	FDualInputChord Result = FDualInputChord(EKeys::Invalid, EKeys::Invalid);
	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		if (bUseEnhancedInputSystem)
		{
			Result.Main = FInputChord(Settings->GetInputKeyForActionEnhanced(ActionEnhanced));
			Result.Alt = FInputChord(Settings->GetInputKeyForActionEnhanced(ActionEnhancedAlt));
		}
		else
		{
			Result.Main = FInputChord(Settings->GetInputKeyForActionTraditional(ActionTraditional));
			Result.Alt = FInputChord(Settings->GetInputKeyForActionTraditional(ActionTraditionalAlt));
		}
	}
	return Result;
}

void USettingsInputKeySelectorWidget::SaveInputToSettings(const FInputChord& NewValue, const bool bIsMain) const
{
	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		if (bUseEnhancedInputSystem)
		{
			Settings->SetInputKeyForActionEnhanced(bIsMain ? ActionEnhanced : ActionEnhancedAlt, NewValue.Key);
		}
		else
		{
			Settings->SetInputKeyForActionTraditional(bIsMain ? ActionTraditional : ActionTraditionalAlt, NewValue.Key);
		}
	}
}

void USettingsInputKeySelectorWidget::HandleNewKeyBinding(const FInputChord& NewValue, const bool bIsMain)
{
	UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings();
	if (!Settings || Settings->IsResettingToDefaults())
	{
		return;
	}

	if (NewValue.Key.IsValid())
	{
		// Check if new key value is already bound to another enhanced action
		{
			const FMappableActionEnhanced OverlappedActionEnhanced = Settings->GetActionForKeyEnhanced(NewValue.Key);
			if (ActionEnhanced != OverlappedActionEnhanced && OverlappedActionEnhanced.IsValid())
			{
				FOnDuplicatedKeyAlertOptionSelected OnKeepButtonClicked;
				TWeakObjectPtr<UJPEGameUserSettings> SettingsWeak = Settings;
				OnKeepButtonClicked.BindLambda([this, NewValue, SettingsWeak, OverlappedActionEnhanced, bIsMain]()
				{
					if (!SettingsWeak.IsValid())
					{
						return;
					}					
					SaveInputToSettings(NewValue, bIsMain);
					SettingsWeak->SetInputKeyForActionEnhanced(OverlappedActionEnhanced, EKeys::Invalid);
					SettingsWeak->OnSettingsLoaded.Broadcast();
				});
				FOnDuplicatedKeyAlertOptionSelected OnRevertButtonClicked;
				OnRevertButtonClicked.BindLambda([this, Settings]()
				{
					Settings->OnSettingsLoaded.Broadcast();
				});
				ShowDuplicatedKeyAlert(NewValue.Key.GetDisplayName(), UJPEInputUtils::GetDisplayNameForActionEnhanced(OverlappedActionEnhanced), OnKeepButtonClicked, OnRevertButtonClicked);
				return;
			}
		}
		// Check if new key value is already bound to another traditional action
		{
			const FMappableActionTraditional OverlappedActionTraditional = Settings->GetActionForKeyTraditional(NewValue.Key);
			if (ActionTraditional != OverlappedActionTraditional && OverlappedActionTraditional.IsValid())
			{
				FOnDuplicatedKeyAlertOptionSelected OnKeepButtonClicked;
				TWeakObjectPtr<UJPEGameUserSettings> SettingsWeak = Settings;
				OnKeepButtonClicked.BindLambda([this, NewValue, SettingsWeak, OverlappedActionTraditional, bIsMain]()
				{
					if (!SettingsWeak.IsValid())
					{
						return;
					}
					SaveInputToSettings(NewValue, bIsMain);
					SettingsWeak->SetInputKeyForActionTraditional(OverlappedActionTraditional, EKeys::Invalid);
					SettingsWeak->OnSettingsLoaded.Broadcast();
				});
				FOnDuplicatedKeyAlertOptionSelected OnRevertButtonClicked;
				OnRevertButtonClicked.BindLambda([this, Settings]()
				{
					Settings->OnSettingsLoaded.Broadcast();
				});
				ShowDuplicatedKeyAlert(NewValue.Key.GetDisplayName(), UJPEInputUtils::GetDisplayNameForActionTraditional(OverlappedActionTraditional), OnKeepButtonClicked, OnRevertButtonClicked);
				return;
			}
		}
	}

	SaveInputToSettings(NewValue, bIsMain);
}

void USettingsInputKeySelectorWidget::ShowDuplicatedKeyAlert(const FText& KeyName, const FText& ActionName, const FOnDuplicatedKeyAlertOptionSelected& OnKeepButtonClicked, const FOnDuplicatedKeyAlertOptionSelected& OnRevertButtonClicked)
{
	if (!DuplicatedKeyAlertWidgetClass)
	{
		if (OnRevertButtonClicked.IsBound())
		{
			OnRevertButtonClicked.Execute();
		}
		return;
	}
	UDuplicatedKeyAlertWidget* Widget = CreateWidget<UDuplicatedKeyAlertWidget>(this, DuplicatedKeyAlertWidgetClass);
	Widget->SetKeyName(KeyName);
	Widget->SetActionName(ActionName);
	Widget->OnKeepButtonClicked = OnKeepButtonClicked;
	Widget->OnRevertButtonClicked = OnRevertButtonClicked;
	Widget->AddToViewport(20);
}
