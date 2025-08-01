// Created by Jorge Puerto. All Rights Reserved.


#include "Common/LabeledCheckbox.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"

bool ULabeledCheckbox::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (LabelTextBlock)
	{
		LabelTextBlock->SetText(LabelText);
	}

	return true;
}

void ULabeledCheckbox::NativeConstruct()
{
	Super::NativeConstruct();

	if (MainButton)
	{
		MainButton->OnClicked.AddUniqueDynamic(this, &ULabeledCheckbox::HandleOnMainButtonClicked);
	}
}

ECheckBoxState ULabeledCheckbox::GetCheckedState()
{
	return MainCheckBox ? MainCheckBox->GetCheckedState() : ECheckBoxState::Undetermined;
}

void ULabeledCheckbox::HandleOnMainButtonClicked()
{
	if (MainCheckBox)
	{
		const ECheckBoxState bNewCheckedState = MainCheckBox->GetCheckedState() != ECheckBoxState::Checked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
		MainCheckBox->SetCheckedState(bNewCheckedState);
		OnCheckedStateChanged.Broadcast(bNewCheckedState);
	}
}
