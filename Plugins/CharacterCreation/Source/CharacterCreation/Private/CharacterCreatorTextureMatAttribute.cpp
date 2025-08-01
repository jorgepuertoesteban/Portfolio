// Created by Jorge Puerto. All Rights Reserved.


#include "CharacterCreatorTextureMatAttribute.h"

void UCharacterCreatorTextureMatAttribute::SetAttributeOnMaterialInstanceDynamic(UMaterialInstanceDynamic* MID, UTexture* Value) const
{
	if (!MID)
	{
		return;
	}
	MID->SetTextureParameterValue(ParameterName, Value);
}
