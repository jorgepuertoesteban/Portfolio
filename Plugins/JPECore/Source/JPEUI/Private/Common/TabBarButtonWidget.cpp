// Created by Jorge Puerto. All Rights Reserved.


#include "Common/TabBarButtonWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTabBarButtonWidget::SetTabName(const FText& NewTabName)
{
	if (TabName.EqualTo(NewTabName))
	{
		return;
	}
	
	TabName = NewTabName;
	Refresh();
}

void UTabBarButtonWidget::Hover() const
{
	Button->SetIsEnabled(false);
}

void UTabBarButtonWidget::Unhover() const
{
	Button->SetIsEnabled(true);
}

void UTabBarButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button)
	{
		Button->OnClicked.AddUniqueDynamic(this, &UTabBarButtonWidget::HandleButtonClicked);
	}
}

bool UTabBarButtonWidget::Refresh()
{
	if (!Label) return false;
	
	Label->SetText(TabName);
	
	return true;
}

void UTabBarButtonWidget::HandleButtonClicked()
{
	OnTabBarButtonClicked.Broadcast(TabName);
}
