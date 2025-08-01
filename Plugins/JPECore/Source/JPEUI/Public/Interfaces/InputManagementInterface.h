// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputManagementInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInputManagementInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JPEUI_API IInputManagementInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ConfigureInput() = 0;
};
