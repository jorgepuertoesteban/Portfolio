// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/JPEHUDInterface.h"
#include "JPEHUD.generated.h"

class UJPEUIConfig;
class UConfirmWidget;
class UAlertWidget;

/**
 *
 */
UCLASS()
class JPEUI_API AJPEHUD : public AHUD, public IJPEHUDInterface
{
	GENERATED_BODY()
public:

	static const FInputModeGameAndUI InputModeGameAndUI;
	static const FInputModeGameOnly InputModeGameOnly;
	static const FInputModeUIOnly InputModeUIOnly;

public:

	//IJPEHUDInterface
	UFUNCTION(BlueprintCallable)
	virtual UAlertWidget* CreateAlert(const FText& Message) override;

	UFUNCTION(BlueprintCallable)
	virtual UAlertWidget* NotifyResponse(bool bIsSuccessful, const FText& Message) override;

	UFUNCTION(BlueprintCallable)
	virtual UConfirmWidget* CreateConfirm(const FText& Message) override;
	//End IJPEHUDInterface

};
