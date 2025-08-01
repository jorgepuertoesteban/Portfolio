// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CCMatAttSetTabContentWidget.h"
#include "CharacterCreatorMatAttributeSet.h"
#include "CharacterCreatorMatAttribute.h"
#include "CharacterCreatorOutfitSlot.h"
#include "Components/ListView.h"
#include "DataHelper/CharacterCreatorMaterialAttributeDH.h"
#include "DataHelper/CharacterCreatorSlotDH.h"
#include "CharacterCreator.h"

bool UCCMatAttSetTabContentWidget::Initialize()
{

	if (Super::Initialize())
	{
		return true;
	}
	return false;
}

void UCCMatAttSetTabContentWidget::SetMaterialAttributesSet(UCharacterCreatorMatAttributesSet* NewAttributesSet, UCharacterCreator* NewCharacterCreator)
{
	MaterialAttributesSet = NewAttributesSet;
	CharacterCreator = NewCharacterCreator;

	AttributesDataHolderArray.Empty();

	if (MaterialAttributesSet && AttributeListView && CharacterCreator)
	{
		for (UCharacterCreatorMatAttribute* CCAttribute : MaterialAttributesSet->Attributes)
		{
			UCharacterCreatorMaterialAttributeDH* DataHolder = NewObject<UCharacterCreatorMaterialAttributeDH>(this);
			DataHolder->CharacterCreator = CharacterCreator;
			DataHolder->CharacterCreatorMaterialAttribute = CCAttribute;
			DataHolder->Value = CharacterCreator->ValueOf(CCAttribute);
			AttributesDataHolderArray.Add(DataHolder);
		}

		AttributeListView->SetListItems(AttributesDataHolderArray);

	}
}