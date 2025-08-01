// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/SettingsMenuWidget.h"

#include "Common/TabBarWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/JPEGameUserSettings.h"
#include "GameFramework/GameUserSettings.h"
#include "Settings/Alerts/RevertAppliedSettingsAlertWidget.h"

#define LOCTEXT_NAMESPACE "SettingsWidget"

void USettingsMenuWidget::SelectTab(const int32 NewTabIndex) const
{
	if (TabBar)
	{
		TabBar->SelectTab(NewTabIndex);
	}
}

void USettingsMenuWidget::ApplyChanges()
{
	UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings();
	if (!Settings)
	{
		return;
	}
	Settings->ApplySettingsSafe();
	if (RevertAppliedSettingsAlertWidgetClass)
	{		
		URevertAppliedSettingsAlertWidget* RevertAppliedSettingsAlertWidget = CreateWidget<URevertAppliedSettingsAlertWidget>(this, RevertAppliedSettingsAlertWidgetClass);
		RevertAppliedSettingsAlertWidget->AddToViewport(20);
	}
}

void USettingsMenuWidget::RevertChanges()
{
	if (UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings())
	{
		GameUserSettings->LoadSettings();
	}
}

void USettingsMenuWidget::ResetToDefaults()
{
	UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings();
	if (!Settings)
	{
		return;
	}
	Settings->ResetToDefaults();
	if (RevertAppliedSettingsAlertWidgetClass)
	{		
		URevertAppliedSettingsAlertWidget* RevertAppliedSettingsAlertWidget = CreateWidget<URevertAppliedSettingsAlertWidget>(this, RevertAppliedSettingsAlertWidgetClass);
		RevertAppliedSettingsAlertWidget->AddToViewport(20);
	}
}

bool USettingsMenuWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (ApplyTextBlock)
		{
			ApplyTextBlock->SetText(LOCTEXT("ApplySettingsChanges", "Apply Changes"));
		}
		if (CloseTextBlock)
		{
			CloseTextBlock->SetText(LOCTEXT("CloseSettingsMenu", "Close"));
		}
		if (RevertTextBlock)
		{
			RevertTextBlock->SetText(LOCTEXT("RevertSettingsChanges", "Revert Changes"));
		}
		if (ResetToDefaultsTextBlock)
		{
			ResetToDefaultsTextBlock->SetText(LOCTEXT("ResetToSettingsDefaults", "Reset to Defaults"));
		}
		return Refresh(0);
	}
	return false;
}

void USettingsMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TabBar)
	{
		TabBar->OnTabSelected.AddUniqueDynamic(this, &USettingsMenuWidget::HandleOnTabSelected);
	}
	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddUniqueDynamic(this, &USettingsMenuWidget::HandleOnApplyButtonClicked);
	}
	if (CloseButton)
	{
		CloseButton->OnClicked.AddUniqueDynamic(this, &USettingsMenuWidget::HandleOnCloseButtonClicked);
	}
	if (RevertButton)
	{
		RevertButton->OnClicked.AddUniqueDynamic(this, &USettingsMenuWidget::HandleOnRevertButtonClicked);
	}
	if (ResetToDefaultsButton)
	{
		ResetToDefaultsButton->OnClicked.AddUniqueDynamic(this, &USettingsMenuWidget::HandleOnResetToDefaultsButtonClicked);
	}
	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		Settings->LoadSettings();
		Settings->OnSettingsApplied.AddUniqueDynamic(this, &USettingsMenuWidget::HandleOnSettingsApplied);
	}
}

bool USettingsMenuWidget::Refresh(const int32 NewTabIndex)
{
	if (!TabBar) return false;
	if (!OptionButtonsPanel) return false;
	if (!ApplyButton) return false;
	if (!CloseButton) return false;
	if (!RevertButton) return false;
	if (!ResetToDefaultsButton) return false;

	TabBar->SelectTab(NewTabIndex);

	const bool bShowOptionButtons = bShowApplyButton || bShowCloseButton || bShowRevertButton || bShowResetToDefaultsButton;
	OptionButtonsPanel->SetVisibility(bShowOptionButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	ApplyButton->SetVisibility(bShowApplyButton ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	CloseButton->SetVisibility(bShowCloseButton ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	RevertButton->SetVisibility(bShowRevertButton ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	ResetToDefaultsButton->SetVisibility(bShowResetToDefaultsButton ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	
	return true;
}

void USettingsMenuWidget::HandleOnTabSelected(const int32 NewTabIndex)
{
	if (TabsWidgetSwitcher)
	{
		TabsWidgetSwitcher->SetActiveWidgetIndex(NewTabIndex);
	}
}

void USettingsMenuWidget::HandleOnApplyButtonClicked()
{
	ApplyChanges();
}

void USettingsMenuWidget::HandleOnCloseButtonClicked()
{
	RevertChanges();
	RemoveFromParent();
}

void USettingsMenuWidget::HandleOnRevertButtonClicked()
{
	RevertChanges();
}

void USettingsMenuWidget::HandleOnResetToDefaultsButtonClicked()
{
	ResetToDefaults();
}

void USettingsMenuWidget::HandleOnSettingsApplied()
{
	FSlateApplication::Get().QueryCursor();
}

#undef LOCTEXT_NAMESPACE
