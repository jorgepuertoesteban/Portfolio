// Created by Jorge Puerto. All Rights Reserved.

#include "OutfitModifiers/CCOM_MorphTarget.h"
#include "Components/CharacterCreatorComponent.h"
#include "CharacterCreatorOutfit.h"

void  UCCOM_MorphTarget::PerformModification(UCharacterCreatorComponent* CharacterCreatorComponent, USkeletalMeshComponent* SkeletalMeshComponent, const UCharacterCreatorOutfit* Outfit, const UCharacterCreator* CharacterCreator) const
{
	if (SkeletalMeshComponent == nullptr)
	{
		return;
	}
	SkeletalMeshComponent->SetMorphTarget(MorphName, Value);
}
