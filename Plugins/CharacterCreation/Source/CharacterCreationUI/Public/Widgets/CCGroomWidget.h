// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCGroomWidget.generated.h"

class UListView;
class UCharacterCreatorGroomsSet;
class UCharacterCreator;
class UCharacterCreatorGroomDH;
class UCharacterCreatorOutfitSlot;

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCCGroomWidget : public UUserWidget
{
	GENERATED_BODY()

public:


	UPROPERTY(meta = (BindWidget))
	UListView* GroomListView;

	UPROPERTY()
	TMap<UCharacterCreatorOutfitSlot*, UCharacterCreatorGroomsSet*> GroomsSetMap;

	UPROPERTY()
	UCharacterCreator* CharacterCreator;

	UPROPERTY()
	TArray<UCharacterCreatorGroomDH*> DataHolderArray;

public:

	virtual bool Initialize() override;

	virtual void SetGrooms(TMap<UCharacterCreatorOutfitSlot*, UCharacterCreatorGroomsSet*> NewGroomsSetMap, UCharacterCreator* NewCharacterCreator);
	
};
