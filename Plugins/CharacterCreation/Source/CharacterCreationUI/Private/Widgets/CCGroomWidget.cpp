// Created by Jorge Puerto. All rights reserved.


#include "Widgets/CCGroomWidget.h"
#include "CharacterCreatorGroom.h"
#include "CharacterCreator.h"
#include "CharacterCreatorGroomsSet.h"
#include "DataHelper/CharacterCreatorGroomDH.h"
#include "Components/ListView.h"
#include "CharacterCreationTypes.h"

bool UCCGroomWidget::Initialize()
{

	if (Super::Initialize())
	{
		return true;
	}
	return false;
}

void UCCGroomWidget::SetGrooms(TMap<UCharacterCreatorOutfitSlot*, UCharacterCreatorGroomsSet*> NewGroomsSetMap, UCharacterCreator* NewCharacterCreator)
{

	GroomsSetMap = NewGroomsSetMap;
	CharacterCreator = NewCharacterCreator;

	DataHolderArray.Empty();

	if (GroomListView && CharacterCreator)
	{

		for (const TPair<UCharacterCreatorOutfitSlot*, UCharacterCreatorGroomsSet*>& pair : GroomsSetMap)
		{
			UCharacterCreatorGroomDH* DataHolder = NewObject<UCharacterCreatorGroomDH>(this);
			DataHolder->CharacterCreator = CharacterCreator;
			DataHolder->GroomsSet = pair.Value;
			DataHolder->GroomSlot = pair.Key;
			DataHolder->SelectedGroom = CharacterCreator->GetSelectedGroom(pair.Key);
			DataHolderArray.Add(DataHolder);
		}

		GroomListView->SetListItems(DataHolderArray);
	}
}
