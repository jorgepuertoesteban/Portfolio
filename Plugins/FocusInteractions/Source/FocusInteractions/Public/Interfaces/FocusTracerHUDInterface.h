// Created by Bionic Ape. All Rights Reserved.

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
};