// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CharacterCreatorAttributeWidget.generated.h"

class UTextBlock;
class UCharacterCreatorAttributeDH;
class UCharacterCreatorSlotDH;
class USlider;
class UCheckBox;
class UListView;

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorAttributeWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	USlider* AttributeSlider;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameTextWidget;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* MorphMinTextWidget;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* MorphMaxTextWidget;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* SliderValueTextWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	const UCharacterCreatorAttribute* CharacterCreatorAttribute;

public:

	virtual bool Initialize() override;

	bool IsListItemSelectable() const override { return true; }

	UFUNCTION()
	void OnValueChanged(float NewValue);

	UFUNCTION(BlueprintCallable)
	void SetAttribute(const UCharacterCreatorAttribute* NewCCAttribute);

	void RefreshWidgetValues();

protected:

	void NativeOnListItemObjectSet(UObject* ListItemObject) override;

};