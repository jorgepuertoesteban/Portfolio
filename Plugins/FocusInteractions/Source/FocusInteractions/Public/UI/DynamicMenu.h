// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DynamicMenu.generated.h"

/**
 * 
 */
UCLASS()
class FOCUSINTERACTIONS_API UDynamicMenu : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Attributes
public:
protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* VisibilityAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimPlaybackSpeed = 3.f;

	bool bWantsToBeVisible = false;
private:
#pragma endregion Attributes

#pragma region Methods
public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual bool TogglePlayVisibilityAnim();
	virtual void HideVisibilityAnim();
protected:


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnVisibilityAnimEnd();
	virtual void OnVisibilityAnimEnd_Implementation() {};
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnVisibilityAnimStart();
	virtual void OnVisibilityAnimStart_Implementation() {};
private:
#pragma endregion Methods 
};
