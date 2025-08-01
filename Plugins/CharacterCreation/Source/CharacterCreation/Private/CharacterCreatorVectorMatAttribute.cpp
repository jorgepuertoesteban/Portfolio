// Created by Jorge Puerto. All Rights Reserved.


#include "CharacterCreatorVectorMatAttribute.h"

void UCharacterCreatorVectorMatAttribute::SetAttributeOnMaterialInstanceDynamic(UMaterialInstanceDynamic* MID, FLinearColor Value ) const 
{
	if (!MID)
	{
		return;
	}
	MID->SetVectorParameterValue(VectorParameterName, Value);

	for (FName ExtraParameterName : ExtraVectorParameterNames)
	{
		MID->SetVectorParameterValue(ExtraParameterName, Value);
	}
}
