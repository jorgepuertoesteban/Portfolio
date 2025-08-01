// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCMatAttSetTabContentWidget.generated.h"


class UCharacterCreatorMatAttributesSet;
class UCharacterCreator;
class UListView;
class UCharacterCreatorMaterialAttributeDH;

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCCMatAttSetTabContentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UListView* AttributeListView;

	//Quick fix for material attribute testing, TODO:Interface/Hierarchy the attributes so they share types
	UPROPERTY()
	UCharacterCreatorMatAttributesSet* MaterialAttributesSet;

	UPROPERTY()
	UCharacterCreator* CharacterCreator;

	UPROPERTY()
	TArray<UCharacterCreatorMaterialAttributeDH*> AttributesDataHolderArray;

public:

	virtual bool Initialize() override;

	virtual void SetMaterialAttributesSet(UCharacterCreatorMatAttributesSet* NewAttributesSet, UCharacterCreator* NewCharacterCreator);
};
