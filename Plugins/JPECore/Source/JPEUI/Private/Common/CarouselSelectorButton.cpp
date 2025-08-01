// Created by Jorge Puerto. All Rights Reserved.


#include "Common/CarouselSelectorButton.h"

#include "Components/Border.h"
#include "Components/Button.h"

void UCarouselSelectorButton::SetOptionIndex(const int32 NewOptionIndex)
{
	OptionIndex = NewOptionIndex;
}

void UCarouselSelectorButton::Hover() const
{
	if (Border)
	{
		Border->SetBrushColor(HoverColor);
	}
}

void UCarouselSelectorButton::Unhover() const
{
	if (Border)
	{
		Border->SetBrushColor(UnhoverColor);
	}
}

void UCarouselSelectorButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button)
	{
		Button->OnClicked.AddUniqueDynamic(this, &UCarouselSelectorButton::HandleOnButtonClicked);
	}
}

void UCarouselSelectorButton::HandleOnButtonClicked()
{
	OnOptionSelected.Broadcast(OptionIndex);
}
