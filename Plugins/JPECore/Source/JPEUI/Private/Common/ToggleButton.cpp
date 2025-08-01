// Created by Jorge Puerto. All Rights Reserved.


#include "Common/ToggleButton.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Slider.h"

void UToggleButton::SetValue(const bool NewValue)
{
	Value = NewValue;
	Refresh(Value);
	OnValueChanged.Broadcast(Value);
}

bool UToggleButton::Initialize()
{
	if (Super::Initialize())
	{
		return GetGameInstance() || Refresh(Value);
	}
	return false;
}

void UToggleButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button)
	{
		Button->OnClicked.AddUniqueDynamic(this, &UToggleButton::HandleOnButtonClicked);
	}
}

bool UToggleButton::Refresh(const float RefreshValue)
{
	if (!Background) return false;
	if (!Toggler) return false;

	Toggler->SetValue(RefreshValue);
	Background->SetBrushColor(FMath::Lerp(OffBackgroundColor, OnBackgroundColor, RefreshValue));
	
	return true;
}

void UToggleButton::HandleOnButtonClicked()
{
	SetValue(!Value);
}