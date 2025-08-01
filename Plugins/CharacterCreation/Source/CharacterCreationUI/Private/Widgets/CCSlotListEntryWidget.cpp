// Created by Jorge Puerto. All rights reserved.


#include "Widgets/CCSlotListEntryWidget.h"
#include "CharacterCreatorOutfit.h"
#include "CharacterCreatorOutfitSlot.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Internationalization/Text.h"
#include "DataHelper/CharacterCreatorSlotDH.h"
#include "Components/Button.h"
#include "CharacterCreator.h"
#include "CharacterCreatorOutfitsSet.h"
#include "CharacterCreatorMatAttribute.h"
#include "Components/CharacterCreatorControlComponent.h"

bool UCCSlotListEntryWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (CheckBox)
		{
			CheckBox->OnCheckStateChanged.AddDynamic(this, &UCCSlotListEntryWidget::OnCheckChanged);
		
			return true;
		}
	}
	return false;
}

void UCCSlotListEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	CCSlotDH = Cast<UCharacterCreatorSlotDH>(ListItemObject);

	if (!CCSlotDH && CCSlotDH->CharacterCreator)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCSlotListEntryWidget::NativeOnListItemObjectSet() ListItemObject empty or not of class UCharacterCreatorOutfitDH"));
		return;
	}

	if (!CheckBox && !CCSlotDH->MaterialAttribute) 
	{
		UE_LOG(LogTemp, Error, TEXT("UCCSlotListEntryWidget::NativeOnListItemObjectSet() CheckBox or CCSlotDH->MaterialAttribute is null"));
		return;
	}

	if (!Label && !CCSlotDH->OutfitSlot)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCSlotListEntryWidget::NativeOnListItemObjectSet() Label or CCSlotDH->OutfitSlot is null"));
		return;
	}

	//If the slot is affected, check it by default
	if(CCSlotDH->MaterialAttributeAffectedSlots.Contains(CCSlotDH->OutfitSlot)) 
	{
		CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
	else 
	{
		CheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}

	Label->SetText(FText::FromName(CCSlotDH->OutfitSlot->FriendlyName));
}

void UCCSlotListEntryWidget::OnCheckChanged(bool NewValue)
{
	//TODO: Maybe move this around so it's no so server dependent

	if (!CCSlotDH)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCSlotListEntryWidget::OnCheckChanged() CCSlotDH empty"));
		return;
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (UCharacterCreatorControlComponent* CCControlComp = Cast<UCharacterCreatorControlComponent>(PC->GetComponentByClass(UCharacterCreatorControlComponent::StaticClass())))
		{
			CCControlComp->SetMaterialAttributeAffectedSlots(CCSlotDH->CharacterCreator, CCSlotDH->MaterialAttribute, CCSlotDH->OutfitSlot, NewValue);
			//CCControlComp->SetMaterialAttributeAffectedSlots(CCSlotDH, NewValue);
		}
	}
}

void UCCSlotListEntryWidget::OnAffectedByOtherMaterialAttribute(UCharacterCreatorSlotDH* OtherCCSlotDH, bool NewValue)
{
	if (!CCSlotDH)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCSlotListEntryWidget::OnAffectedByOtherMaterialAttribute() CCSlotDH empty"));
		return;
	}

	if (!OtherCCSlotDH->MaterialAttributeAffectedSlots.Contains(CCSlotDH->OutfitSlot)) 
	{
		return;
	}

	OnCheckChanged(NewValue);
}