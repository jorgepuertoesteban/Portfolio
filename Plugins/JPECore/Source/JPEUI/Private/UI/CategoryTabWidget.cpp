// Created by Jorge Puerto. All rights reserved.


#include "UI/CategoryTabWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"


bool UCategoryTabWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (TabButton)
		{
			TabButton->OnClicked.AddDynamic(this, &UCategoryTabWidget::OnButtonClicked);
			Refresh();
			return true;
		}
	}
	return false;
}

void UCategoryTabWidget::Setup(const FText& NewTabCategoryName, UObject* NewResourceObject)
{
	TabCategoryName = NewTabCategoryName;
	ResourceObject = NewResourceObject;
	Refresh();
}

void UCategoryTabWidget::Refresh()
{
	if (TabTextBlock)
	{
		TabTextBlock->SetText(TabCategoryName);
	}
	if (ResourceObject)
	{
		ButtonStyleSelected.Normal.SetResourceObject(ResourceObject);
		ButtonStyleSelected.Disabled.SetResourceObject(ResourceObject);
		ButtonStyleSelected.Hovered.SetResourceObject(ResourceObject);
		ButtonStyleSelected.Pressed.SetResourceObject(ResourceObject);

		ButtonStyleNotSelected.Normal.SetResourceObject(ResourceObject);
		ButtonStyleNotSelected.Disabled.SetResourceObject(ResourceObject);
		ButtonStyleNotSelected.Hovered.SetResourceObject(ResourceObject);
		ButtonStyleNotSelected.Pressed.SetResourceObject(ResourceObject);
		
		TabButton->SetStyle(ButtonStyleNotSelected);
	}
}

void UCategoryTabWidget::OnButtonClicked()
{
	TabSelectedEvent.Broadcast(this);
}