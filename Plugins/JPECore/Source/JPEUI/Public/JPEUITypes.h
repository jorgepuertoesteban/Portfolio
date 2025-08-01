// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"



UENUM(BlueprintType)
enum class EViewportRenderType : uint8
{
	DEFAULT_VIEWPORT,
	BLEND_VIEWPORT,
	MASKED_VIEWPORT,
};

