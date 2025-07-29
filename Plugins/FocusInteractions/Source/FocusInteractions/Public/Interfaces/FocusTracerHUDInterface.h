// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "FocusTracerHUDInterface.generated.h"

/**
*
*/
UINTERFACE(Blueprintable)
class FOCUSINTERACTIONS_API UFocusTracerHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class IFocusTracerHUDInterface
{
	GENERATED_BODY()

public:
	virtual void HideFocusTraceInterface() = 0;
	virtual void ShowFocusTraceInterface() = 0;
};