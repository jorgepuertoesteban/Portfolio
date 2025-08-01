// Created by Jorge Puerto. All rights reserved.


#include "Widgets/CCOutfitWidget.h"
#include "CharacterCreator.h"
#include "CharacterCreatorOutfitsSet.h"
#include "DataHelper/CharacterCreatorOutfitDH.h"
#include "Components/ListView.h"

bool UCCOutfitWidget::Initialize()
{

	if (Super::Initialize())
	{
		return true;
	}
	return false;
}

void UCCOutfitWidget::SetOutfits(TMap<UCharacterCreatorOutfitSlot*, UCharacterCreatorOutfitsSet*> NewOutfitsSetMap, UCharacterCreator* NewCharacterCreator)
{

	OutfitsSetMap = NewOutfitsSetMap;
	CharacterCreator = NewCharacterCreator;

	DataHolderArray.Empty();

	if (OutfitListView && CharacterCreator)
	{

		for (const TPair<UCharacterCreatorOutfitSlot*, UCharacterCreatorOutfitsSet*>& pair : OutfitsSetMap)
		{
			UCharacterCreatorOutfitDH* DataHolder = NewObject<UCharacterCreatorOutfitDH>(this);
			DataHolder->CharacterCreator = CharacterCreator;
			DataHolder->OutfitsSet = pair.Value;
			DataHolder->OutfitSlot = pair.Key;
			DataHolder->SelectedOutfit = CharacterCreator->GetSelectedOutfit(pair.Key);
			DataHolderArray.Add(DataHolder);
		}

		OutfitListView->SetListItems(DataHolderArray);
	}
}
