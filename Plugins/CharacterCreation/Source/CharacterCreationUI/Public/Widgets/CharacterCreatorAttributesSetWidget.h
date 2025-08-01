// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterCreatorAttributesSetWidget.generated.h"

class UListView;
class UPanelWidget;
class UCharacterCreatorAttributesSet;
class UCharacterCreatorAttributeWidget;
class UCharacterCreatorOutfitsSet;


/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorAttributesSetWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidgetOptional))
	UListView* AttributeListView;

	UPROPERTY(meta = (BindWidgetOptional))
	UPanelWidget* PanelWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UCharacterCreatorAttributesSet* AttributesSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCharacterCreatorAttributeWidget> AttributeWidgetClass;

public:
	
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	virtual void SetAttributesSet(const UCharacterCreatorAttributesSet* NewAttributesSet);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UCharacterCreatorOutfitsSet* OutfitsSet;

	virtual void RefreshWidget();
};
