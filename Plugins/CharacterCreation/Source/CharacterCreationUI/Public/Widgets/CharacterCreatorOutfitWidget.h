// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CharacterCreatorOutfitWidget.generated.h"

class UButton;
class UCharacterCreatorOutfit;
class UImage;
class UTextBlock;
class UCharacterCreatorOutfitVariantWidget;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutfitSelectedEvent);

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorOutfitWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UCharacterCreatorOutfit* Outfit;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* SelectedOutfitName;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
	UButton* SubmitButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UPanelWidget* VariantPanelWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCharacterCreatorOutfitVariantWidget> OutfitVariantWidgetClass;

public:

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void SetOutfit(const UCharacterCreatorOutfit* NewOutfit);

	virtual void RefreshWidget();

	UFUNCTION(BlueprintCallable)
	void OnButtonPressed();

	virtual bool IsListItemSelectable() const override;

	FOnOutfitSelectedEvent OnOutfitSelectedEvent;

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

};
