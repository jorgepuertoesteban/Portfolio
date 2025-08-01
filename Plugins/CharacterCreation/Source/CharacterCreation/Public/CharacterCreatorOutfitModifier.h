// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorOutfitModifier.generated.h"

class UCharacterCreator;
class UCharacterCreatorOutfit;
class UCharacterCreatorComponent;
class USkeletalMeshComponent;

UCLASS(BlueprintType)
class CHARACTERCREATION_API UCharacterCreatorOutfitModifier : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "CharacterCreatorCondition")
	virtual void PerformModification(UCharacterCreatorComponent* CharacterCreatorComponent, USkeletalMeshComponent* SkeletalMeshComponent, const UCharacterCreatorOutfit* Outfit, const UCharacterCreator* CharacterCreator) const {}

};
