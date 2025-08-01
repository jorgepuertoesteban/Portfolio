// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/SettingsItemWidget.h"

#include "Components/Border.h"
#include "Components/PanelWidget.h"
#include "GameFramework/JPEGameUserSettings.h"

bool USettingsItemWidget::Initialize()
{
	if (Super::Initialize())
	{
		return Refresh();
	}
	return false;
}

void USettingsItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UJPEGameUserSettings* Settings = UJPEGameUserSettings::GetJPEGameUserSettings())
	{
		Settings->OnSettingsLoaded.AddUniqueDynamic(this, &USettingsItemWidget::HandleOnJPEGameUserSettingsLoadedOrApplied);
		Settings->OnSettingsApplied.AddUniqueDynamic(this, &USettingsItemWidget::HandleOnJPEGameUserSettingsLoadedOrApplied);
	}
}

bool USettingsItemWidget::Refresh()
{
	if (!Background) return false;
	
	if (const UPanelWidget* Parent = Cast<UPanelWidget>(GetParent()))
	{
		const int32 ChildIndex = Parent->GetChildIndex(this);
		Background->SetBrushColor(ChildIndex % 2 == 0 ? OddRowColor : EvenRowColor); // Remember: row 0 is the first row (odd)
	}
	
	return true;
}

void USettingsItemWidget::HandleOnJPEGameUserSettingsLoadedOrApplied()
{
	UpdateUIValues();
}
