// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CharacterCreatorMaterialVariant.h"
#include "CharacterCreatorOutfitVariantWidget.generated.h"

class UButton;
class UCharacterCreatorOutfit;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorOutfitVariantWidget : public UUserWidget
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UCharacterCreatorOutfit* Outfit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterCreatorMaterialVariant OutfitVariant;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* SelectedOutfitName;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
	UButton* SubmitButton;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 VariantIndex = -1;

public:

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void SetValues(const FCharacterCreatorMaterialVariant& NewOutfitVariant, const UCharacterCreatorOutfit* NewOutfit, const int32 NewIndex);

	virtual void RefreshWidget();

	UFUNCTION()
	void OnButtonPressed();

};
