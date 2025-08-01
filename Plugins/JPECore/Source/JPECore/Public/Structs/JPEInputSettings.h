// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "JPEInputSettings.generated.h"

USTRUCT(BlueprintType)
struct FJPEInputSettings
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "JPEInput")
	FName ActionName;
	UPROPERTY(VisibleAnywhere, Category = "JPEInput")
	FKey Key;
	UPROPERTY(VisibleAnywhere, Category = "JPEInput")
	float DeadZone;
	UPROPERTY(VisibleAnywhere, Category = "JPEInput")
	float Sensitivity;
	UPROPERTY(VisibleAnywhere, Category = "JPEInput")
	bool bInvert;

	FJPEInputSettings()
		: DeadZone(0.2f)
		, Sensitivity(1.f)
		, bInvert(false)
	{}

};
