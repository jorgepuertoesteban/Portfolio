// Created by Jorge Puerto. All Rights Reserved.

#include "Helpers/TabHelper.h"

#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/CategoryTabWidget.h"

bool UTabHelper::Initialize(
	TSubclassOf<UCategoryTabWidget> NewTabWidgetClass,
	UWidgetSwitcher* NewTabContentSwitcher,
	UPanelWidget* NewTabsWidget,
	UTextBlock* NewSelectedCategoryText)
{
	TabWidgetClass = NewTabWidgetClass;
	TabContentSwitcher = NewTabContentSwitcher;
	ButtonsPanelWidget = NewTabsWidget;
	CategorySelectedText = NewSelectedCategoryText;//Optional!

	return TabWidgetClass && TabContentSwitcher && ButtonsPanelWidget;
}

void UTabHelper::CreateTabs()
{
	if (TabWidgetClass && TabContentSwitcher && ButtonsPanelWidget && CategorySelectedText)
	{
		TabContentSwitcher->ClearChildren();
		ButtonsPanelWidget->ClearChildren();
		CategorySelectedText->SetText(FText());

		bool bIsFirstTab = true;

		for (FCreateCategoryTabParams& Params : CategoryTabParamsArray)
		{
			UPanelSlot* SwitcherSlot = TabContentSwitcher->AddChild(Params.ContentWidget);

			////Create Button for the tab
			if (UCategoryTabWidget* TabWidget = CreateWidget<UCategoryTabWidget>(ButtonsPanelWidget, TabWidgetClass))
			{
				Params.TabWidget = TabWidget;
				TabWidget->Setup(Params.TabCategoryName, Params.ResourceObject);
				UPanelSlot* ButtonPanelSlot = ButtonsPanelWidget->AddChild(TabWidget);
				TabWidget->OnChanged().AddUObject(this, &UTabHelper::SetActiveTab);
				if (bIsFirstTab)//Hack to set the correct Style
				{
					bIsFirstTab = false;
					TabWidget->SetStyleSelected();
					if (CategorySelectedText)
					{
						CategorySelectedText->SetText(Params.TabCategoryName);
					}
				}
				OnCategoryTabCreated.Broadcast(TabWidget, Params.ContentWidget);//Use this to Set the Panel Slots and many more initialization stuff
			}
		}
	}
}


void UTabHelper::Empty()
{
	CategoryTabParamsArray.Empty();
}

void UTabHelper::SetActiveTab(UCategoryTabWidget* SelectedCategoryTabWidget)
{
	if (SelectedCategoryTabWidget)
	{
		//for (UCategoryTabWidget* TabWidget : TabWidgetsArray)
		for (const FCreateCategoryTabParams& Params : CategoryTabParamsArray)
		{
			if (Params.TabWidget == SelectedCategoryTabWidget)
			{
				Params.TabWidget->SetStyleSelected();
				if (CategorySelectedText)
				{
					CategorySelectedText->SetText(Params.TabCategoryName);
				}
				if (TabContentSwitcher)
				{
					TabContentSwitcher->SetActiveWidget(Params.ContentWidget);
				}
			}
			else
			{
				Params.TabWidget->SetStyleNotSelected();
			}
		}
	}
}

void UTabHelper::NextTab()
{
	if (!TabContentSwitcher)
	{
		UE_LOG(LogTemp, Error, TEXT("TabContentSwitcher is null!"));
		return;
	}

	if (const UWidget* SelectedContent = TabContentSwitcher->GetActiveWidget())
	{
		for (int32 i = 0; i < CategoryTabParamsArray.Num(); i++)
		{
			if (CategoryTabParamsArray[i].ContentWidget == SelectedContent)
			{
				const int32 NewContentIndex = (i + 1) % CategoryTabParamsArray.Num();
				SetActiveTabByIndex(NewContentIndex);
			}
		}
	}
}

void UTabHelper::PreviousTab()
{
	if (!TabContentSwitcher)
	{
		UE_LOG(LogTemp, Error, TEXT("TabContentSwitcher is null!"));
		return;
	}

	if (const UWidget* SelectedContent = TabContentSwitcher->GetActiveWidget())
	{
		for (int32 i = 0; i < CategoryTabParamsArray.Num(); i++)
		{
			if (CategoryTabParamsArray[i].ContentWidget == SelectedContent)
			{
				const int32 NewContentIndex = i == 0 ? CategoryTabParamsArray.Num() - 1 : i - 1;
				SetActiveTabByIndex(NewContentIndex);
			}
		}
	}
}

void UTabHelper::SetActiveTabByIndex(int32 NewContentIndex)
{
	if (CategoryTabParamsArray.IsValidIndex(NewContentIndex))
	{
		SetActiveTab(CategoryTabParamsArray[NewContentIndex].TabWidget);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UTabHelper::SetActiveTabByIndex NewContentIndex is invalid index!"));
	}
}
