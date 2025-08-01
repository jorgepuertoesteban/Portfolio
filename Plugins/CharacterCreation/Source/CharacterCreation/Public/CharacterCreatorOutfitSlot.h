// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreationTypes.h"
#include "CharacterCreatorOutfitSlot.generated.h"

class UCharacterCreatorOutfit;
/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class CHARACTERCREATION_API UCharacterCreatorOutfitSlot : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	FString Name;
	
	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	FName FriendlyName;

	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	FString AttachementName;
	
	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	UCharacterCreatorOutfit* DefaultOutfit;

	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	TArray<FName> Tags;
	
	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	bool bIsRoot = false;

	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	bool bSpawnsComponent = true;

	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	bool bSetLeaderPoseFromRoot = true;
	
	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	bool bRefreshGrooms = false;

	UPROPERTY(EditAnywhere, Category = "CharacterCreation")
	FName NonSpawnedComponentTag;
};
