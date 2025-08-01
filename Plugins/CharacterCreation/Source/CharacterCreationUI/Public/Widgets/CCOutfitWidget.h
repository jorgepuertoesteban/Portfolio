// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCOutfitWidget.generated.h"

class UListView;
class UCharacterCreatorOutfitsSet;
class UCharacterCreator;
class UCharacterCreatorOutfitDH;
class UCharacterCreatorOutfitSlot;

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCCOutfitWidget : public UUserWidget
{
	GENERATED_BODY()

public:


	UPROPERTY(meta = (BindWidget))
	UListView* OutfitListView;

	UPROPERTY()
	TMap<UCharacterCreatorOutfitSlot*, UCharacterCreatorOutfitsSet*> OutfitsSetMap;

	UPROPERTY()
	UCharacterCreator* CharacterCreator;

	UPROPERTY()
	TArray<UCharacterCreatorOutfitDH*> DataHolderArray;

public:

	virtual bool Initialize() override;

	virtual void SetOutfits(TMap<UCharacterCreatorOutfitSlot*, UCharacterCreatorOutfitsSet*> NewOutfitsSetMap, UCharacterCreator* NewCharacterCreator);
	
};
