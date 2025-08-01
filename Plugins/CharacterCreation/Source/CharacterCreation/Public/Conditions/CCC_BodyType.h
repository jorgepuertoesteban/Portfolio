// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CharacterCreatorCondition.h"
#include "CharacterCreationTypes.h"
#include "CCC_BodyType.generated.h"

UCLASS(EditInlineNew)
class CHARACTERCREATION_API UCCC_BodyType : public UCharacterCreatorCondition
{
	GENERATED_BODY()

	
public:

	virtual bool IsConditionMet(const UCharacterCreatorOutfit* Outfit, const UCharacterCreator* CharacterCreator) const override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCharacterCreationBodyType BodyType;

};
