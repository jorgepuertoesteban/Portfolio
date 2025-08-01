// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CCGroomListEntryWidget.generated.h"

class UTextBlock;
class UCharacterCreatorGroomDH;
class UButton;

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONUI_API UCCGroomListEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	UButton* LeftButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RightButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameTextWidget;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SelectedGroomName;

	UPROPERTY()
	UCharacterCreatorGroomDH* CCGroomDH;

public:

	virtual bool Initialize() override;

	bool IsListItemSelectable() const override { return CCGroomDH != nullptr; }

	UFUNCTION()
	void OnLeftClicked();

	UFUNCTION()
	void OnRightClicked();

	void SetGroom(bool bDirectionIsLeft);

protected:

	void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void UpdateGroomText();
};