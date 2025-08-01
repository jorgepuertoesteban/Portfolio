// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreationTypes.h"
#include "CharacterCreatorTextureMatAttribute.generated.h"

/**
 * 
 */

class UCharacterCreatorOutfitSlot;
class UTexture;

UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorTextureMatAttribute : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditInstanceOnly)
	FName ParameterName;

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
	void SetAttributeOnMaterialInstanceDynamic(UMaterialInstanceDynamic* MID, UTexture* Value) const ;


};
