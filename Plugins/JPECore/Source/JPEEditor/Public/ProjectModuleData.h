// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectModuleData.generated.h"

USTRUCT()
struct FProjectModuleData
{
	GENERATED_BODY()
public:
	UPROPERTY()
		FString Name;
	UPROPERTY()
		FString Type;
	UPROPERTY()
		FString LoadingPhase;
};