// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/ActionListWidget.h"
#include "ActionListItemWidget.generated.h"

class UButton;
class UTextBlock;
class UActionType_Trpg;
struct FActionByActionsSetResult;
class UActionListInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClic);
/**
 * 
 */
UCLASS()
class FOCUSINTERACTIONS_API UActionListItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
#pragma region Attributes
public:

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnClic OnClicked;
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ActionButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UActionListInfo* ActionInfo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ActionName;
private:
#pragma endregion Attributes

#pragma region Methods
public:
	virtual bool Initialize() override;

	UFUNCTION()
	virtual void OnButtonClicked();

	UFUNCTION(BlueprintCallable)
	void SetIsButtonEnabled(bool bIsButtonEnabled);

	//UFUNCTION()
	//void Configure();
protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
private:
#pragma endregion Methods 
};
