// Created by Jorge Puerto. All rights reserved.


#include "Widgets/CCBodyWidget.h"
#include "CharacterCreatorGroom.h"
#include "CharacterCreator.h"
#include "CharacterCreatorGroomsSet.h"
#include "DataHelper/CharacterCreatorGroomDH.h"
#include "Components/ComboBoxString.h"
#include "Components/CharacterCreatorControlComponent.h"
#include "CharacterCreationTypes.h"

bool UCCBodyWidget::Initialize()
{

	if (Super::Initialize())
	{
		if (BodySettingsComboBox) {
			BodySettingsComboBox->OnSelectionChanged.AddDynamic(this, &UCCBodyWidget::OnSelectionChanged);
			return true;
		}
	}
	return false;
}

void UCCBodyWidget::SetCharacterCreator(UCharacterCreator* NewCharacterCreator)
{
	CharacterCreator = NewCharacterCreator;
	if (BodySettingsComboBox && CharacterCreator)
	{
		FString OptionName;

		for (uint8 i = 0; i < (uint8)FCharacterCreationBodyType::MAX; ++i)
		{
			switch ((FCharacterCreationBodyType)i)
			{
			case FCharacterCreationBodyType::NormalWeight:
				OptionName = "Normal weight"; //Veras como nos cancelan por llamarlo normal i.i
				break;
			case FCharacterCreationBodyType::OverWeight:
				OptionName = "Over weight";
				break;
			case FCharacterCreationBodyType::UnderWeight:
				OptionName = "Under weight";
				break;
			default:
				break;
			}
			BodySettingsComboBox->AddOption(OptionName);
		}

		BodySettingsComboBox->SetSelectedIndex((uint8)CharacterCreator->BodyType);
	}
}

void UCCBodyWidget::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (UCharacterCreatorControlComponent* CCControlComp = Cast<UCharacterCreatorControlComponent>(PC->GetComponentByClass(UCharacterCreatorControlComponent::StaticClass())))
		{
			FCharacterCreationBodyType NewBodyType = FCharacterCreationBodyType::MAX;
			if (SelectedItem == "Normal weight")
			{
				NewBodyType = FCharacterCreationBodyType::NormalWeight;
			}
			else if (SelectedItem == "Over weight")
			{
				NewBodyType = FCharacterCreationBodyType::OverWeight;
			}
			else if (SelectedItem == "Under weight")
			{
				NewBodyType = FCharacterCreationBodyType::UnderWeight;
			}
			CCControlComp->SetBodyType(CharacterCreator, NewBodyType);
		}
	}
}

//void UCCBodyWidget::SetGrooms(TMap<UCharacterCreatorOutfitSlot*, UCharacterCreatorGroomsSet*> NewGroomsSetMap, UCharacterCreator* NewCharacterCreator)
//{
//
//	GroomsSetMap = NewGroomsSetMap;
//	CharacterCreator = NewCharacterCreator;
//
//	DataHolderArray.Empty();
//
//	if (GroomListView && CharacterCreator)
//	{
//
//		for (const TPair<UCharacterCreatorOutfitSlot*, UCharacterCreatorGroomsSet*>& pair : GroomsSetMap)
//		{
//			pair.Key;
//			pair.Value;
//
//			UCharacterCreatorGroomDH* DataHolder = NewObject<UCharacterCreatorGroomDH>(this);
//			DataHolder->CharacterCreator = CharacterCreator;
//			DataHolder->GroomsSet = pair.Value;
//			DataHolder->GroomSlot = pair.Key;
//			DataHolder->SelectedGroom = CharacterCreator->GetSelectedGroom(pair.Key);
//			DataHolderArray.Add(DataHolder);
//		}
//
//		GroomListView->SetListItems(DataHolderArray);
//	}
//}
