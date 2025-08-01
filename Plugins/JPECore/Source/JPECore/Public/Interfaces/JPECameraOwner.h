// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "JPECameraOwner.generated.h"

class UInventory;
class UConfirmWidget;
class UAlertWidget;

/**
*
*/
UINTERFACE(Blueprintable)
class JPECORE_API UJPECameraOwner : public UInterface
{
	GENERATED_BODY()
};

class IJPECameraOwner
{
	GENERATED_BODY()

public:

	virtual bool IsViewTargetSetInCameraManager() const = 0;
};