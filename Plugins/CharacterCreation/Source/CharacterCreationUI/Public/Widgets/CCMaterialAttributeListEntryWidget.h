// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CCMaterialAttributeListEntryWidget.generated.h"

class UTextBlock;
class UCharacterCreatorMaterialAttributeDH;
class UCharacterCreatorSlotDH;
class USlider;
class UCheckBox;
class UListView;

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCCMaterialAttributeListEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	USlider* MorphSlider;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameTextWidget;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MorphMinTextWidget;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MorphMaxTextWidget;

	UPROPERTY(meta = (BindWidget))
	UListView* PermitedSlotsListView;

	UPROPERTY()
	UCharacterCreatorMaterialAttributeDH* CCMaterialAttributeDH;

	//Might not be the best place to handle permitedslot creation
	UPROPERTY()
	TArray<UCharacterCreatorSlotDH*> SlotsDataHolderArray;

public:

	virtual bool Initialize() override;

	bool IsListItemSelectable() const override { return CCMaterialAttributeDH != nullptr; }

	UFUNCTION()
	void OnValueChanged(float NewValue);

protected:

	void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//void NativeOnItemSelectionChanged(bool bIsSelected) override;
	//void NativeOnItemExpansionChanged(bool bIsExpanded) override;
	//void NativeOnEntryReleased() override;
};