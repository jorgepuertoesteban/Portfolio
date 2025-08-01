// Created by Jorge Puerto. All Rights Reserved.


#include "JPEUIBlueprintFunctionLibrary.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"

void UJPEUIBlueprintFunctionLibrary::SetOptionsTab(UPanelWidget* ButtonsPanel, UWidgetSwitcher* TabSwitcher, UButton* SelectedTabButton, const FLinearColor& SelectedTabColor /*= FLinearColor::White*/, const FLinearColor& UnselectedTabColor)
{
	if (!ButtonsPanel || !TabSwitcher || !SelectedTabButton)
	{
		//TODO: Log
		return;
	}

	for (int32 i = 0; i < ButtonsPanel->GetChildrenCount(); i++)
	{
		UButton* Button = Cast<UButton>(ButtonsPanel->GetChildAt(i));
		if (Button)
		{
			if (Button == SelectedTabButton)
			{
				Button->SetBackgroundColor(SelectedTabColor);
				TabSwitcher->SetActiveWidgetIndex(i);
			}
			else
			{
				Button->SetBackgroundColor(UnselectedTabColor);
			}
		}
	}
}
