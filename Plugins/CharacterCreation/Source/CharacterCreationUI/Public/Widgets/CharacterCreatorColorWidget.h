// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CharacterCreatorColorWidget.generated.h"

class UButton;
class UCharacterCreatorGroom;
class UImage;
class UTextBlock;
class UCharacterCreatorGroomVariantWidget;
class UCharacterCreatorVectorMatAttribute;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnColorSelectedEvent);

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorColorWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UCharacterCreatorVectorMatAttribute* VectorMatAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
	FLinearColor Color;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
	UButton* SubmitButton;

public:

	virtual bool Initialize() override;
	
	UFUNCTION(BlueprintCallable)
	void SetVectorMatAttribute(const UCharacterCreatorVectorMatAttribute* NewVectorMatAttribute);

	UFUNCTION(BlueprintCallable)
	void SetColor(FLinearColor NewColor);

	virtual void RefreshWidget();

	UFUNCTION()
	void OnButtonPressed();

	bool IsListItemSelectable() const override;

	FOnColorSelectedEvent OnColorSelectedEvent;
};
