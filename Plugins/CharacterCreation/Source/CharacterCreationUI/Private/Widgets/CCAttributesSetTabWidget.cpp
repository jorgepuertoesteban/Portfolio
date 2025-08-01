// Created by Jorge Puerto. All rights reserved.


#include "Widgets/CCAttributesSetTabWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

bool UCCAttributesSetTabWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (TabTextBlock && TabButton)
		{
			TabButton->OnClicked.AddDynamic(this, &UCCAttributesSetTabWidget::OnButtonClicked);
			return true;
		}
	}
	return false;
}

void UCCAttributesSetTabWidget::OnButtonClicked()
{
	TabSelectedEvent.Broadcast();
}

void UCCAttributesSetTabWidget::SetTabText(FText TabText)
{
	TabTextBlock->SetText(TabText);
}
