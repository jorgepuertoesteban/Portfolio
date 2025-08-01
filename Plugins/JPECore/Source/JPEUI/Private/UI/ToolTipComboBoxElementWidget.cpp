// Created by Jorge Puerto. All Rights Reserved.


#include "UI/ToolTipComboBoxElementWidget.h"

#include "Components/TextBlock.h"

void UToolTipComboBoxElementWidget::SetItem(const FString& Item)
{
	if (!DataByItem.Contains(Item))
	{
		return;
	}
	if (NameTextBlock)
	{
		NameTextBlock->SetText(DataByItem[Item].FriendlyName);
	}
	SetToolTipText(DataByItem[Item].Description);
}
