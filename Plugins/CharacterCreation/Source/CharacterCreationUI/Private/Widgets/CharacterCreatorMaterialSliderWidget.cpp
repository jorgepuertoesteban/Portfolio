// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CharacterCreatorMaterialSliderWidget.h"

#include "CharacterCreator.h"
#include "CharacterCreatorMatAttribute.h"
#include "Blueprint/UserWidget.h"
#include "Components/CharacterCreatorControlComponent.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

bool UCharacterCreatorMaterialSliderWidget::Initialize()
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

	SliderWidget->OnValueChanged.AddDynamic(this, &UCharacterCreatorMaterialSliderWidget::OnFloatValueChanged);

	RefreshWidget();

	return true;
}

void UCharacterCreatorMaterialSliderWidget::SetMaterialAttribute(const UCharacterCreatorMatAttribute* NewMatAttribute)
{
	MatAttribute = NewMatAttribute;
	RefreshWidget();
}

void UCharacterCreatorMaterialSliderWidget::RefreshWidget()
{
	if (!MatAttribute)
	{
		return;
	}
	
	SliderWidget->SetMinValue(MatAttribute->ValueMin);
	SliderWidget->SetMaxValue(MatAttribute->ValueMax);
	if (MaterialAttributeTextBlock)
	{
		MaterialAttributeTextBlock->SetText(MatAttribute->FriendlyName);
	}

	const UCharacterCreatorControlComponent* CCControlComp = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(GetOwningPlayer());
	if (!CCControlComp || !CCControlComp->MainCharacterCreator)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCOutfitListEntryWidget::OnValueChanged() No UCharacterCreatorControlComponent Found"));
		return;
	}
	for (const FCCMaterialAttributeValue& MaterialAttribute : CCControlComp->MainCharacterCreator->MaterialAttributeValues)
	{
		if (MaterialAttribute.MaterialAttribute == MatAttribute)
		{
			SliderWidget->SetValue(MaterialAttribute.Value);
			break;
		}
	}
}

void UCharacterCreatorMaterialSliderWidget::OnFloatValueChanged(float Value)
{
	UCharacterCreatorControlComponent* CCControlComp = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(GetOwningPlayer());
	if (!CCControlComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCOutfitListEntryWidget::OnValueChanged() No UCharacterCreatorControlComponent Found"));
		return;
	}

	CCControlComp->Server_SetMaterialAttributeValue(CCControlComp->MainCharacterCreator, MatAttribute, SliderWidget->GetValue());
	OnMatAttributeChangedEvent.Broadcast();
}
