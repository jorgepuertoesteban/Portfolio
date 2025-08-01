// Created by Jorge Puerto. All rights reserved.


#include "UI/CategoryTabContentWidget.h"

#include "Components/PanelWidget.h"

void UCategoryTabContentWidget::AddEntry(UWidget* EntryWidget) const
{
	if (ContentPanel)
	{
		ContentPanel->AddChild(EntryWidget);
	}
}

TArray<UWidget*> UCategoryTabContentWidget::GetAllEntries() const
{
	return ContentPanel ? ContentPanel->GetAllChildren() : TArray<UWidget*>();
}

void UCategoryTabContentWidget::ClearEntries() const
{
	if (ContentPanel)
	{
		ContentPanel->ClearChildren();
	}
}
