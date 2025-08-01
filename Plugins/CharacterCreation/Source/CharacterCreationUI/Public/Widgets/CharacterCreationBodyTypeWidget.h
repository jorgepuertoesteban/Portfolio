// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterCreationTypes.h"
#include "Blueprint/UserWidget.h"
#include "CharacterCreationBodyTypeWidget.generated.h"

class USlider;

UCLASS(Abstract)
class CHARACTERCREATIONUI_API UCharacterCreationBodyTypeWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCharacterCreationBodyType> Options;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	USlider* OptionsSlider;

	UPROPERTY(Transient)
	int32 CurrentOptionIndex;

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual void OnOptionsSliderValueChanged(float NewValue);

};
