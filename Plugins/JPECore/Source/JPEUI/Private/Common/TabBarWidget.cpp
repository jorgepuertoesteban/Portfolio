// Created by Jorge Puerto. All Rights Reserved.


#include "Common/TabBarWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/PanelWidget.h"
#include "Fonts/SlateFontInfo.h"
#include "Common/TabBarButtonWidget.h"
#include "Components/HorizontalBoxSlot.h"

void UTabBarWidget::SetTabNames(const TArray<FText>& NewTabNames)
{
	TabNames = NewTabNames;
	Refresh();
}

void UTabBarWidget::SelectTab(const int32 NewTabIndex)
{
	if (ActiveTabIndex == NewTabIndex)
	{
		return;
	}
	
	// Update Button
	if (TabButtons.IsValidIndex(ActiveTabIndex))
	{
		TabButtons[ActiveTabIndex]->Unhover();
	}
	if (TabButtons.IsValidIndex(NewTabIndex))
	{
		TabButtons[NewTabIndex]->Hover();
	}
	
	// Notify tab changed
	ActiveTabIndex = NewTabIndex;
	OnTabSelected.Broadcast(ActiveTabIndex);
}

bool UTabBarWidget::Initialize()
{
	if (Super::Initialize())
	{
		return Refresh();
	}
	return false;
}

void UTabBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (UTabBarButtonWidget* TabButton : TabButtons)
	{
		TabButton->OnTabBarButtonClicked.AddUniqueDynamic(this, &UTabBarWidget::HandleOnTabBarButtonClicked);
	}
	
	SelectTab(0);
}

bool UTabBarWidget::Refresh()
{
	if (!TabButtonWidgetClass) return false;
	if (!TabButtonsPanel) return false;
	
	// Initialize Tab Buttons
	TabButtonsPanel->ClearChildren();
	TabButtons.Empty();
	for (FText TabName : TabNames)
	{
		UTabBarButtonWidget* TabButton = Cast<UTabBarButtonWidget>(CreateWidget(this, TabButtonWidgetClass, FName("TabButton_" + TabName.ToString())));
		TabButton->SetTabName(TabName);
		TabButtons.Add(TabButton);
		TabButtonsPanel->AddChild(TabButton);
		if (UHorizontalBoxSlot* TabButtonSlot = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(TabButton))
		{
			TabButtonSlot->SetSize(ESlateSizeRule::Fill);
		}
	}
	
	return true;
}

void UTabBarWidget::HandleOnTabBarButtonClicked(const FText& TabName)
{
	int32 Index = 0;
	for (FText AuxTabName : TabNames)
	{
		if (AuxTabName.EqualTo(TabName))
		{
			SelectTab(Index);
			break;
		}
		Index++;
	}
	
}
