// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreationTypes.h"
#include "CharacterCreatorVectorMatAttribute.generated.h"

class UCharacterCreatorOutfitSlot;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorVectorMatAttribute : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditInstanceOnly)
	FName VectorParameterName;
	
	UPROPERTY(EditInstanceOnly)
	TArray<FName> ExtraVectorParameterNames;

	UPROPERTY(EditInstanceOnly)
	FText FriendlyName = FText::GetEmpty();

	UPROPERTY(EditInstanceOnly)
	FString DatabaseColumnName;

	UPROPERTY(EditInstanceOnly)
	TArray<const UCharacterCreatorOutfitSlot*> PermitedSlots;

	UPROPERTY(EditInstanceOnly)
	const UCharacterCreatorOutfitSlot* TargetSlot;

public: 
	
	UFUNCTION()
	void SetAttributeOnMaterialInstanceDynamic(UMaterialInstanceDynamic* MID, FLinearColor Value) const ;
};
