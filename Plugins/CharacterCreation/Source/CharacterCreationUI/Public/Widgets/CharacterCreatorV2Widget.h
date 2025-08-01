// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterCreatorV2Widget.generated.h"

class UCharacterCreatorModel;
class UButton;

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorV2Widget : public UUserWidget
{
	GENERATED_BODY()


protected:

	UPROPERTY(EditAnywhere)
	UCharacterCreatorModel* CharacterCreatorModel;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* SaveButton;

public:

	virtual bool Initialize() override;

	UFUNCTION()
	void OnSaveButtonPressed();
	
};
