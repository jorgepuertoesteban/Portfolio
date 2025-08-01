// Created by Jorge Puerto. All rights reserved.


#include "Widgets/CCMaterialAttributeListEntryWidget.h"
#include "CharacterCreatorMatAttribute.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "Components/Slider.h"
#include "Components/ListView.h"
#include "DataHelper/CharacterCreatorMaterialAttributeDH.h"
#include "DataHelper/CharacterCreatorSlotDH.h"
#include "CharacterCreator.h"
#include "Components/CharacterCreatorControlComponent.h"

bool UCCMaterialAttributeListEntryWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (MorphSlider)
		{
			MorphSlider->OnValueChanged.AddDynamic(this, &UCCMaterialAttributeListEntryWidget::OnValueChanged);
			return true;
		}
	}
	return false;
}

void UCCMaterialAttributeListEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	CCMaterialAttributeDH = Cast<UCharacterCreatorMaterialAttributeDH>(ListItemObject);

	if (!CCMaterialAttributeDH && CCMaterialAttributeDH->CharacterCreatorMaterialAttribute)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCMaterialAttributeListEntryWidget::NativeOnListItemObjectSet() ListItemObject empty or not of class UCharacterCreatorMaterialAttribute"));
		return;
	}

	NameTextWidget->SetText(CCMaterialAttributeDH->CharacterCreatorMaterialAttribute->FriendlyName);
	MorphMinTextWidget->SetText(FText::FromString(FString::SanitizeFloat(CCMaterialAttributeDH->CharacterCreatorMaterialAttribute->ValueMin)));
	MorphMaxTextWidget->SetText(FText::FromString(FString::SanitizeFloat(CCMaterialAttributeDH->CharacterCreatorMaterialAttribute->ValueMax)));
	MorphSlider->SetValue(CCMaterialAttributeDH->Value);
	MorphSlider->SetMaxValue(CCMaterialAttributeDH->CharacterCreatorMaterialAttribute->ValueMax);
	MorphSlider->SetMinValue(CCMaterialAttributeDH->CharacterCreatorMaterialAttribute->ValueMin);

	SlotsDataHolderArray.Empty();

	//This might not be placed here, probably its better to isolate this on a class inherited from ListView and manage item creation there
	//Create permited slot content list DataHolder and feed it to the listview
	for (const UCharacterCreatorOutfitSlot* PermitedSlot : CCMaterialAttributeDH->CharacterCreatorMaterialAttribute->PermitedSlots)
	{
		if (PermitedSlot != CCMaterialAttributeDH->CharacterCreatorMaterialAttribute->TargetSlot)
		{
			UCharacterCreatorSlotDH* SlotDataHolder = NewObject<UCharacterCreatorSlotDH>(this);
			SlotDataHolder->CharacterCreator = CCMaterialAttributeDH->CharacterCreator;
			SlotDataHolder->MaterialAttribute = CCMaterialAttributeDH->CharacterCreatorMaterialAttribute;
			SlotDataHolder->OutfitSlot = PermitedSlot;

			SlotsDataHolderArray.Add(SlotDataHolder);
		}
	}

	PermitedSlotsListView->SetListItems(SlotsDataHolderArray);
}

void UCCMaterialAttributeListEntryWidget::OnValueChanged(float NewValue)
{
	//TODO: Maybe move this around so it's no so server dependent

	if (!CCMaterialAttributeDH)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCMaterialAttributeListEntryWidget::OnValueChanged() CCAttribute empty"));
		return;
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (UCharacterCreatorControlComponent* CCControlComp = Cast<UCharacterCreatorControlComponent>(PC->GetComponentByClass(UCharacterCreatorControlComponent::StaticClass())))
		{
			CCControlComp->SetMaterialAttributeValue(CCMaterialAttributeDH->CharacterCreator, CCMaterialAttributeDH->CharacterCreatorMaterialAttribute, NewValue);
		}
	}
}
