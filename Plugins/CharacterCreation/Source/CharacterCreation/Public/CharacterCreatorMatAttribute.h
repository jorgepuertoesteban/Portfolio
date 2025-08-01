// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreationTypes.h"
#include "CharacterCreatorMatAttribute.generated.h"

/**
 * 
 */

class UCharacterCreatorOutfitSlot;

UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorMatAttribute : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditInstanceOnly)
	FName ScalarParameterName;

	UPROPERTY(EditInstanceOnly)
	FText FriendlyName = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	float ValueMin = -1.f;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	float ValueMax = 1.f;

	UPROPERTY(EditInstanceOnly)
	FString DatabaseColumnName;

	UPROPERTY(EditInstanceOnly)
	TArray<const UCharacterCreatorOutfitSlot*> PermitedSlots;

	UPROPERTY(EditInstanceOnly)
	const UCharacterCreatorOutfitSlot* TargetSlot;


public: 
	
	UFUNCTION()
	void SetAttributeOnMaterialInstanceDynamic(UMaterialInstanceDynamic* MID, float Value) const ;


};
