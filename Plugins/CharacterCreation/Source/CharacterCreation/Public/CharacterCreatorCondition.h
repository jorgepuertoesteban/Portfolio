// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCreatorCondition.generated.h"

class UCharacterCreator;
class UCharacterCreatorOutfit;

UCLASS(hidecategories = Object, BlueprintType)
class CHARACTERCREATION_API UCharacterCreatorCondition : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "CharacterCreatorCondition")
	virtual bool IsConditionMet(const UCharacterCreatorOutfit* Outfit, const UCharacterCreator* CharacterCreator) const { return true; }


};
