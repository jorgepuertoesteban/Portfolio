// Created by Jorge Puerto. All rights reserved.


#include "Widgets/CCAttributeListEntryWidget.h"

#include "CharacterCreatorAttribute.h"
#include "Components/CharacterCreatorControlComponent.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "DataHelper/CharacterCreatorAttributeDH.h"
#include "Internationalization/Text.h"

bool UCCAttributeListEntryWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (MorphSlider)
		{
			MorphSlider->OnValueChanged.AddDynamic(this, &UCCAttributeListEntryWidget::OnValueChanged);
			return true;
		}
	}
	return false;
}

void UCCAttributeListEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	CCAttributeDH = Cast<UCharacterCreatorAttributeDH>(ListItemObject);

	if (!CCAttributeDH && CCAttributeDH->CharacterCreatorAttribute)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCAttributeListEntryWidget::NativeOnListItemObjectSet() ListItemObject empty or not of class UCharacterCreatorAttribute"));
		return;
	}

	NameTextWidget->SetText(FText::FromName(CCAttributeDH->CharacterCreatorAttribute->FriendlyName));
	MorphMinTextWidget->SetText(FText::FromString(FString::SanitizeFloat(CCAttributeDH->CharacterCreatorAttribute->MorphMin)));
	MorphMaxTextWidget->SetText(FText::FromString(FString::SanitizeFloat(CCAttributeDH->CharacterCreatorAttribute->MorphMax)));
	MorphSlider->SetValue(CCAttributeDH->Value);
	MorphSlider->SetMaxValue(CCAttributeDH->CharacterCreatorAttribute->MorphMax);
	MorphSlider->SetMinValue(CCAttributeDH->CharacterCreatorAttribute->MorphMin);

	SlotsDataHolderArray.Empty();

}

void UCCAttributeListEntryWidget::OnValueChanged(float NewValue)
{
	//TODO: Maybe move this around so it's no so server dependent

	if (!CCAttributeDH)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCAttributeListEntryWidget::OnValueChanged() CCAttribute empty"));
		return;
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		
		if (UCharacterCreatorControlComponent* CCControlComp = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(PC))
		{
			CCControlComp->SetAttributeValue(CCAttributeDH->CharacterCreator, CCAttributeDH->CharacterCreatorAttribute, NewValue);
		}
	}
}
