// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputModePawnInterface.generated.h"

UENUM(BlueprintType)
enum class EInputMode : uint8
{
	GameAndUI,
	GameOnly,
	UIOnly,
};

UINTERFACE(Blueprintable)
class JPEUI_API UInputModePawnInterface : public UInterface
{
	GENERATED_BODY()
};

class JPEUI_API IInputModePawnInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EInputMode GetInputMode() const;

	virtual EInputMode GetInputMode_Implementation() const { return EInputMode::GameOnly; };

	
};
