// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CCSlotListEntryWidget.generated.h"

class UTextBlock;
class UCharacterCreatorSlotDH;
class UButton;
class UCheckBox;


/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCCSlotListEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	UCheckBox* CheckBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Label;

	UPROPERTY()
	UCharacterCreatorSlotDH* CCSlotDH;

public:

	virtual bool Initialize() override;

	bool IsListItemSelectable() const override { return CCSlotDH != nullptr; }

	UFUNCTION()
	void OnCheckChanged(bool NewValue);

	UFUNCTION()
	void OnAffectedByOtherMaterialAttribute(UCharacterCreatorSlotDH* OtherCCSlotDH, bool NewValue);

protected:

	void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};