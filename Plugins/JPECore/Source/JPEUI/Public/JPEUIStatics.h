// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JPEUIStatics.generated.h"

class APlayerController;

UCLASS()
class JPEUI_API UJPEUIStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static void CalculatePositionInViewport(APlayerController* PlayerController, float const SizeX, float const SizeY, FVector2D& Result);

};