// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CharacterCreatorGroomWidget.generated.h"

class UButton;
class UCharacterCreatorGroom;
class UImage;
class UTextBlock;
class UCharacterCreatorGroomVariantWidget;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGroomSelectedEvent);

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorGroomWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
	const UCharacterCreatorGroom* Groom;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* SelectedGroomName;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
	UButton* SubmitButton;



public:

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void SetGroom(const UCharacterCreatorGroom* NewGroom);

	virtual void RefreshWidget();

	UFUNCTION(BlueprintCallable)
	void OnButtonPressed();

	bool IsListItemSelectable() const override;

	FOnGroomSelectedEvent OnGroomSelectedEvent;

protected:

	void NativeOnListItemObjectSet(UObject* ListItemObject) override;

};
