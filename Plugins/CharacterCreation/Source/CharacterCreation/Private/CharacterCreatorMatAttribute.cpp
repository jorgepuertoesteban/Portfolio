// Created by Jorge Puerto. All Rights Reserved.


#include "CharacterCreatorMatAttribute.h"

void UCharacterCreatorMatAttribute::SetAttributeOnMaterialInstanceDynamic(UMaterialInstanceDynamic* MID, float Value ) const 
{
	if (!MID)
	{
		return;
	}
	MID->SetScalarParameterValue(ScalarParameterName, FMath::Clamp(Value,ValueMin,ValueMax));
}
