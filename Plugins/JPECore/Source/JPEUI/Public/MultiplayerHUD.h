// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MultiplayerHUD.generated.h"

/**
 * 
 */
UCLASS(Config = JPE)
class JPEUI_API AMultiplayerHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(Config)
	TSubclassOf<class UUserWidget> MenuClass;

	UPROPERTY(Config)
	TSubclassOf<class UUserWidget> InGameMenuClass;

protected:

	void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();
};
