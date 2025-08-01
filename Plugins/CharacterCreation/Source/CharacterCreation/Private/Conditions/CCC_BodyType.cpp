// Created by Jorge Puerto. All Rights Reserved.

#include "Conditions/CCC_BodyType.h"
#include "CharacterCreator.h"

bool UCCC_BodyType::IsConditionMet(const UCharacterCreatorOutfit* Outfit, const UCharacterCreator* CharacterCreator) const
{
	if (CharacterCreator == nullptr)
	{
		return false;

	}
	return CharacterCreator->BodyType == BodyType;
}