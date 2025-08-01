// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterCreatorGroomsSetWidget.generated.h"

class UCharacterCreatorGroomsSet;
class UCharacterCreatorGroomWidget;
class UPanelWidget;

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorGroomsSetWidget : public UUserWidget
{
	GENERATED_BODY()
public:	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UCharacterCreatorGroomsSet* GroomsSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCharacterCreatorGroomWidget> GroomWidgetClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* PanelWidget;

public:

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void SetGroomsSet(const UCharacterCreatorGroomsSet* NewGroomsSet);

	virtual void RefreshWidget();


	UFUNCTION(BlueprintImplementableEvent)
	void GroomSelected();


};
