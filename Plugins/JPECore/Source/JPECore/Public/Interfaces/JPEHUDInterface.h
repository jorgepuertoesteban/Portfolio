// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "JPEHUDInterface.generated.h"

class UInventory;
class UConfirmWidget;
class UAlertWidget;

/**
*
*/
UINTERFACE(Blueprintable)
class JPECORE_API UJPEHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class IJPEHUDInterface
{
	GENERATED_BODY()

public:

	virtual UAlertWidget* CreateAlert(const FText& Message) = 0;

	virtual UAlertWidget* NotifyResponse(bool bIsSuccessful, const FText& Message) = 0;

	virtual UConfirmWidget* CreateConfirm(const FText& Message) = 0;
};