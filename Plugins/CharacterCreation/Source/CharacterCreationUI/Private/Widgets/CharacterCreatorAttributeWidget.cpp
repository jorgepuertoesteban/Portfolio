// Created by Jorge Puerto. All rights reserved.


#include "Widgets/CharacterCreatorAttributeWidget.h"
#include "CharacterCreatorAttribute.h"
#include "CharacterCreatorMatAttribute.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "Components/Slider.h"
#include "Components/ListView.h"
#include "DataHelper/CharacterCreatorAttributeDH.h"
#include "DataHelper/CharacterCreatorSlotDH.h"
#include "CharacterCreator.h"
#include "Components/CharacterCreatorControlComponent.h"

bool UCharacterCreatorAttributeWidget::Initialize()
{
	if (!Super::Initialize())
	{
		UE_LOG(LogTemp, Error, TEXT("Super::Initialize is false"));
		return false;
	}
	if (!AttributeSlider)
	{
		UE_LOG(LogTemp, Error, TEXT("MorphSlider is null"));
		return false;
	}
	AttributeSlider->OnValueChanged.AddDynamic(this, &UCharacterCreatorAttributeWidget::OnValueChanged);
	RefreshWidgetValues();
	return true;
}

void UCharacterCreatorAttributeWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	SetAttribute(Cast<UCharacterCreatorAttribute>(ListItemObject));
}

void UCharacterCreatorAttributeWidget::RefreshWidgetValues()
{
	if (!CharacterCreatorAttribute)
	{
		UE_LOG(LogTemp, Log, TEXT("CharacterCreatorAttribute is null"));
		return;
	}
	AttributeSlider->SetMaxValue(CharacterCreatorAttribute->MorphMax);
	AttributeSlider->SetMinValue(CharacterCreatorAttribute->MorphMin);

	if (NameTextWidget)//Optional
	{
		NameTextWidget->SetText(FText::FromName(CharacterCreatorAttribute->FriendlyName));
	}
	if (MorphMinTextWidget)//Optional
	{
		MorphMinTextWidget->SetText(FText::FromString(FString::SanitizeFloat(CharacterCreatorAttribute->MorphMin)));
	}
	if (MorphMaxTextWidget)//Optional
	{
		MorphMaxTextWidget->SetText(FText::FromString(FString::SanitizeFloat(CharacterCreatorAttribute->MorphMax)));
	}

	UCharacterCreatorControlComponent* ControlComp = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(GetOwningPlayer());
	if (!ControlComp || !ControlComp->MainCharacterCreator)
	{
		UE_LOG(LogTemp, Error, TEXT("ControlComp is null"));
		return;
	}

	float const AttributeValue = ControlComp->MainCharacterCreator->ValueOf(CharacterCreatorAttribute);
	AttributeSlider->SetValue(AttributeValue);
	if (SliderValueTextWidget)//Optional
	{
		SliderValueTextWidget->SetText(FText::FromString(FString::SanitizeFloat(AttributeValue, 2)));
	}
}

void UCharacterCreatorAttributeWidget::OnValueChanged(float NewValue)
{
	UCharacterCreatorControlComponent* ControlComp = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(GetOwningPlayer());
	if (!ControlComp)
	{
		UE_LOG(LogTemp, Error, TEXT("ControlComp is null"));
		return;
	}
	ControlComp->SetAttributeValue(ControlComp->MainCharacterCreator, CharacterCreatorAttribute, NewValue);

	if (SliderValueTextWidget)//Optional
	{
		SliderValueTextWidget->SetText(FText::FromString(FString::SanitizeFloat(NewValue, 2)));
	}
}

void UCharacterCreatorAttributeWidget::SetAttribute(const UCharacterCreatorAttribute* NewCCAttribute)
{
	CharacterCreatorAttribute = NewCCAttribute;
	RefreshWidgetValues();
}
