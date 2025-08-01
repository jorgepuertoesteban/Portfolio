// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/PlayerController.h"
#include "HUDComponent.generated.h"

class IJPEHUDInterface;
class UUserWidget;
class ULocalPlayer;

/**
 * Please try to avoid using this class directly. Using it will require your module to reference the JPEUI module, which might not be necessary.
 *
 * Instead, use this class as a template to create your own HUD components.
 *
 * Let's aim to reduce unnecessary inheritance!
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JPEUI_API UHUDComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	static const FInputModeGameAndUI InputModeGameAndUI;
	static const FInputModeGameOnly InputModeGameOnly;
	static const FInputModeUIOnly InputModeUIOnly;

public:

	UFUNCTION(BlueprintCallable)
	APlayerController* GetController() const;

	UFUNCTION(BlueprintCallable)
	APawn* GetControlledPawn() const;
	
	UFUNCTION(BlueprintCallable)
	AHUD* GetHUD() const;
	
	UFUNCTION(BlueprintCallable)
	ULocalPlayer* GetOwningLocalPlayer() const;

	TScriptInterface<IJPEHUDInterface> GetJPEHUD() const;

	UUserWidget* NewWidget(const TSubclassOf<UUserWidget> WidgetClass) const;
	static void ShowWidget(UUserWidget* Widget);
	static void HideWidget(UUserWidget* Widget);

	UFUNCTION(BlueprintCallable)
	void SetInputModeGameAndUI();

	UFUNCTION(BlueprintCallable)
	void SetInputModeGameOnly();
	
	UFUNCTION(BlueprintCallable)
	void SetInputModeUIOnly();

};
