// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterCreatorGroomColorsSetWidget.generated.h"

class UCharacterCreatorColorsSet;
class UCharacterCreatorColorWidget;
class UCharacterCreatorVectorMatAttribute;
class UPanelWidget;

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorGroomColorsSetWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UCharacterCreatorVectorMatAttribute* VectorMatAttribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UCharacterCreatorColorsSet* ColorsSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCharacterCreatorColorWidget> ColorWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* PanelWidget;

public:	
	
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void SetColorsSet(const UCharacterCreatorColorsSet* NewColorsSet);

	UFUNCTION(BlueprintCallable)
	void SetVectorMatAttribute(const UCharacterCreatorVectorMatAttribute* NewVectorMatAttribute);

	virtual void RefreshWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void ColorSelected();
};
