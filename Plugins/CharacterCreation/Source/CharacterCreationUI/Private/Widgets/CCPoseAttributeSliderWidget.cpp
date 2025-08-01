// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CCPoseAttributeSliderWidget.h"
#include "CharacterCreator.h"
#include "CharacterCreatorPoseAttribute.h"
#include "Blueprint/UserWidget.h"
#include "Components/CharacterCreatorControlComponent.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

bool UCCPoseAttributeSliderWidget::Initialize()
{
	if (!Super::Initialize())
	{
		UE_LOG(LogTemp, Error, TEXT("UCategoryTabContentWidget::Initialize() Super::Initialize failed"));
		return false;
	}
	
	if (!SliderWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SliderWidget is null"));
		return false;
	}

	SliderWidget->OnValueChanged.AddDynamic(this, &UCCPoseAttributeSliderWidget::OnFloatValueChanged);

	RefreshWidget();

	return true;
}

void UCCPoseAttributeSliderWidget::RefreshWidget()
{
	if (!PoseAttribute)
	{
		return;
	}
	SliderWidget->SetMinValue(-1.f);
	SliderWidget->SetMaxValue(1.f);
	SliderWidget->SetValue(0.f);

	if (PoseAttributeTextBlock)
	{
		PoseAttributeTextBlock->SetText(PoseAttribute->FriendlyName);
	}
}

void UCCPoseAttributeSliderWidget::OnFloatValueChanged(float Value)
{
	UCharacterCreatorControlComponent* CCControlComp = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(GetOwningPlayer());
	if (!CCControlComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCOutfitListEntryWidget::OnValueChanged() No UCharacterCreatorControlComponent Found"));
		return;
	}
	CCControlComp->Server_SetPoseAttributeValue(CCControlComp->MainCharacterCreator, PoseAttribute, SliderWidget->GetValue());
}
