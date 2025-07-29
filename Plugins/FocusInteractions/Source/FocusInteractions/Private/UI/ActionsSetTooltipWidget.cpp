// Created by Jorge Puerto. All Rights Reserved.


#include "UI/ActionsSetTooltipWidget.h"
#include "Components/TextBlock.h"

void UActionsSetTooltipWidget::SetTooltipText(const FText& NewTooltipText)
{
	if (TooltipTextBlock)
	{
		TooltipTextBlock->SetText(NewTooltipText);
	}
}
