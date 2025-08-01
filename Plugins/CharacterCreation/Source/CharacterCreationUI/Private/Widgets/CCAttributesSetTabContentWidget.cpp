// Created by Jorge Puerto. All rights reserved.


#include "Widgets/CCAttributesSetTabContentWidget.h"
#include "CharacterCreatorAttributesSet.h"
#include "Components/ListView.h"
#include "DataHelper/CharacterCreatorAttributeDH.h"
#include "CharacterCreator.h"

bool UCCAttributesSetTabContentWidget::Initialize()
{

	if (Super::Initialize())
	{
		return true;
	}
	return false;
}

void UCCAttributesSetTabContentWidget::SetAttributesSet(UCharacterCreatorAttributesSet* NewAttributesSet, UCharacterCreator* NewCharacterCreator)
{
	AttributesSet = NewAttributesSet;
	CharacterCreator = NewCharacterCreator;

	AttributesDataHolderArray.Empty();

	if (AttributesSet && AttributeListView && CharacterCreator)
	{
		for (const UCharacterCreatorAttribute* CCAttribute : AttributesSet->Attributes)
		{
			UCharacterCreatorAttributeDH* DataHolder = NewObject<UCharacterCreatorAttributeDH>(this);
			DataHolder->CharacterCreator = CharacterCreator;
			DataHolder->CharacterCreatorAttribute = CCAttribute;
			DataHolder->Value = CharacterCreator->ValueOf(CCAttribute);
			AttributesDataHolderArray.Add(DataHolder);
		}

		AttributeListView->SetListItems(AttributesDataHolderArray);
	}
}
