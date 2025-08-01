// Created by Jorge Puerto. All Rights Reserved.


#include "Common/AdvancedComboBoxString.h"

#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"

void UAdvancedComboBoxString::AddOption(const FString& Option)
{
	if (ComboBox)
	{
		ComboBox->AddOption(Option);
	}
}

void UAdvancedComboBoxString::ClearOptions()
{
	if (ComboBox)
	{
		ComboBox->ClearOptions();
	}
}

void UAdvancedComboBoxString::SetSelectedOption(const FString& Option)
{
	if (ComboBox)
	{
		ComboBox->SetSelectedOption(Option);
	}
	if (CustomOptionText)
	{
		CustomOptionText->SetText(FText::FromString(Option));
	}
}

void UAdvancedComboBoxString::SetSelectedIndex(const int32 Index)
{
	if (ComboBox)
	{
		ComboBox->SetSelectedIndex(Index);
	}
}

int32 UAdvancedComboBoxString::GetSelectedIndex() const
{
	if (ComboBox)
	{
		return ComboBox->GetSelectedIndex();
	}
	return INDEX_NONE;
}

void UAdvancedComboBoxString::NativeConstruct()
{
	Super::NativeConstruct();

	if (ComboBox)
	{
		ComboBox->OnSelectionChanged.AddUniqueDynamic(this, &UAdvancedComboBoxString::HandleOnComboBoxSelectionChanged);
	}
}

void UAdvancedComboBoxString::HandleOnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (CustomOptionText)
	{
		CustomOptionText->SetText(FText::FromString(SelectedItem));
	}

	OnSelectionChanged.Broadcast(SelectedItem, SelectionType);
}
