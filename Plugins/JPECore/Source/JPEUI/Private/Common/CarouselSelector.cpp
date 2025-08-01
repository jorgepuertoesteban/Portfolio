// Created by Jorge Puerto. All Rights Reserved.


#include "Common/CarouselSelector.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Common/CarouselSelectorButton.h"

void UCarouselSelector::SetOptions(const TArray<FString>& NewOptions)
{
	Options = NewOptions;
	Refresh();
}

void UCarouselSelector::SelectOption(const int32 NewOptionIndex)
{
	if (Options.IsValidIndex(NewOptionIndex))
	{
		SelectOption(Options[NewOptionIndex]);
	}
}

void UCarouselSelector::SelectOption(const FString& NewOption)
{
	const int32 NewOptionIndex = Options.Find(NewOption);
	
	if (const UCarouselSelectorButton* Button = Cast<UCarouselSelectorButton>(ButtonsPanel->GetChildAt(SelectedOptionIndex)))
	{
		Button->Unhover();
	}
	if (const UCarouselSelectorButton* Button = Cast<UCarouselSelectorButton>(ButtonsPanel->GetChildAt(NewOptionIndex)))
	{
		Button->Hover();
	}
	if (CurrentOptionText)
	{
		CurrentOptionText->SetText(FText::FromString(NewOption));
	}
	
	SelectedOptionIndex = NewOptionIndex;
	OnSelectionChanged.Broadcast(SelectedOptionIndex);
}

bool UCarouselSelector::Initialize()
{
	if (Super::Initialize())
	{
		return Refresh();
	}
	return false;
}

void UCarouselSelector::NativeConstruct()
{
	Super::NativeConstruct();

	ArrowLeft->OnClicked.AddUniqueDynamic(this, &UCarouselSelector::HandleOnArrowLeftClicked);
	ArrowRight->OnClicked.AddUniqueDynamic(this, &UCarouselSelector::HandleOnArrowRightClicked);
}

bool UCarouselSelector::Refresh()
{
	if (!CarouselSelectorButtonClass) return false;
	if (!ButtonsPanel) return false;
	if (Options.IsEmpty()) return false;

	ButtonsPanel->ClearChildren();
	for (int32 i = 0; i < Options.Num(); ++i)
	{
		const FString Option = Options[i];
		UCarouselSelectorButton* Button = Cast<UCarouselSelectorButton>(CreateWidget(this, CarouselSelectorButtonClass));
		Button->SetOptionIndex(i);
		Button->Unhover();
		Button->OnOptionSelected.AddDynamic(this, &UCarouselSelector::HandleOnOptionSelected);
		ButtonsPanel->AddChild(Button);
		if (UHorizontalBoxSlot* ButtonSlot = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(Button))
		{
			ButtonSlot->SetSize(ESlateSizeRule::Fill);
			ButtonSlot->SetPadding(FMargin(5.f, 0.f, 5.f, 0.f));
		}
	}
	
	SelectOption(0);
	
	return true;
}

void UCarouselSelector::HandleOnArrowLeftClicked()
{
	const int32 NumOptions = Options.Num();
	if (NumOptions <= 0)
	{
		return;
	}

	const int32 NewOptionIndex = SelectedOptionIndex == INDEX_NONE ? 0 : (SelectedOptionIndex + NumOptions - 1) % NumOptions;
	SelectOption(NewOptionIndex);
}

void UCarouselSelector::HandleOnArrowRightClicked()
{
	const int32 NumOptions = Options.Num();
	if (NumOptions <= 0)
	{
		return;
	}

	const int32 NewOptionIndex = SelectedOptionIndex == INDEX_NONE ? 0 : (SelectedOptionIndex + 1) % NumOptions;
	SelectOption(NewOptionIndex);
}

void UCarouselSelector::HandleOnOptionSelected(const int32 NewOptionIndex)
{
	SelectOption(NewOptionIndex);
}
