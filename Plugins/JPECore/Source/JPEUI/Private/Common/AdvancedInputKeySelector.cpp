// Created by Jorge Puerto. All Rights Reserved.


#include "Common/AdvancedInputKeySelector.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/InputKeySelector.h"
#include "Components/TextBlock.h"

void UAdvancedInputKeySelector::SetSelectedKey(const FInputChord& NewKey) const
{
	InputKeySelector->SetSelectedKey(NewKey);
	UpdateVisuals();
}

void UAdvancedInputKeySelector::SetText(const FText& NewText) const
{
	if (InputKeyImage)
	{
		InputKeyImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (InputKeyText)
	{
		InputKeyText->SetVisibility(ESlateVisibility::HitTestInvisible);
		InputKeyText->SetText(NewText);
	}
}

void UAdvancedInputKeySelector::SetTexture(UTexture2D* NewTexture) const
{
	if (InputKeyText)
	{
		InputKeyText->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (InputKeyImage)
	{
		InputKeyImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		InputKeyImage->SetBrushFromTexture(NewTexture);
	}
}

void UAdvancedInputKeySelector::UpdateVisuals() const
{
	if (!InputKeySelector) return;

	const bool bIsSelectingKey = InputKeySelector->GetIsSelectingKey();
	InputKeyBorder->SetVisibility(bIsSelectingKey ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	if (bIsSelectingKey)
	{	
		SetText(InputKeySelector->GetKeySelectionText());
	}
	else if(InputKeySelector->GetSelectedKey().Key.IsValid())
	{
		if (UTexture2D* const * KeyTexture = InputKeyTextures.Find(InputKeySelector->GetSelectedKey().Key))
		{
			SetTexture(*KeyTexture);
		}
		else
		{
			SetText(InputKeySelector->GetSelectedKey().Key.IsModifierKey()
				? InputKeySelector->GetSelectedKey().Key.GetDisplayName()
				: InputKeySelector->GetSelectedKey().GetInputText());
		}
	}
	else
	{
		SetText(InputKeySelector->GetNoKeySpecifiedText());
	}
}

bool UAdvancedInputKeySelector::Initialize()
{
	if (Super::Initialize())
	{
		if (!InputKeySelector) return false;
		
        InputKeySelector->SetAllowGamepadKeys(false);
		FTextBlockStyle TextBlockStyle;
		TextBlockStyle.ColorAndOpacity = FSlateColor(FColor(0,0,0,0));
		InputKeySelector->SetTextStyle(TextBlockStyle);
		
		return true;
	}
	return false;
}

void UAdvancedInputKeySelector::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (InputKeySelector)
	{
		UpdateVisuals();
		InputKeySelector->OnKeySelected.AddUniqueDynamic(this, &UAdvancedInputKeySelector::HandleOnInputKeySelected);
		InputKeySelector->OnIsSelectingKeyChanged.AddUniqueDynamic(this, &UAdvancedInputKeySelector::HandleOnOnIsSelectingKeyChanged);
	}
}

void UAdvancedInputKeySelector::HandleOnInputKeySelected(const FInputChord SelectedKey)
{
	UpdateVisuals();
	OnKeySelected.Broadcast(SelectedKey);
}

void UAdvancedInputKeySelector::HandleOnOnIsSelectingKeyChanged()
{
	UpdateVisuals();
	OnIsSelectingKeyChanged.Broadcast(InputKeySelector->GetIsSelectingKey());
}
