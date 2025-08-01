// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterCreatorOutfitsSetWidget.generated.h"

class UCharacterCreatorOutfitsSet;
class UCharacterCreatorOutfitWidget;
class UPanelWidget;

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorOutfitsSetWidget : public UUserWidget
{
	GENERATED_BODY()
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
	const UCharacterCreatorOutfitsSet* OutfitsSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
	TSubclassOf<UCharacterCreatorOutfitWidget> OutfitWidgetClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* PanelWidget;

public:

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void SetOutfitsSet(const UCharacterCreatorOutfitsSet* NewOutfitsSet);

	virtual void RefreshWidget();


	UFUNCTION(BlueprintImplementableEvent)
	void OutfitSelected();


};
