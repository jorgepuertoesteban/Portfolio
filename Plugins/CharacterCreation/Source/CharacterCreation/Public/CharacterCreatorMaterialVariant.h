// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterCreatorMaterialVariant.generated.h"

class UMaterialInterface;

USTRUCT(BlueprintType)
struct FCharacterCreatorMaterialVariant
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor Thumbnail = FLinearColor(0.f, 0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray< TSoftObjectPtr<UMaterialInterface>> Materials;
};
