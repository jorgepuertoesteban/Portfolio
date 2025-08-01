// Created by Jorge Puerto. All Rights Reserved.


#include "DataValidation/CharacterCreatorOutfitValidator.h"
#include "CharacterCreatorOutfit.h"

bool UCharacterCreatorOutfitValidator::CanValidateAsset_Implementation(UObject* InAsset) const
{
	return (InAsset != nullptr) && (UCharacterCreatorOutfit::StaticClass() == InAsset->GetClass());
}

EDataValidationResult UCharacterCreatorOutfitValidator::ValidateLoadedAsset_Implementation(UObject* InAsset, TArray<FText>& ValidationErrors)
{
	check(UCharacterCreatorOutfit::StaticClass() == InAsset->GetClass());

	UCharacterCreatorOutfit* CharacterCreatorOutfit = Cast<UCharacterCreatorOutfit>(InAsset);

	if (CharacterCreatorOutfit == nullptr)
	{
		return EDataValidationResult::Valid;
	}

	if (CharacterCreatorOutfit->Slot == nullptr)
	{
		AssetFails(InAsset, FText::FromString(FString("Slot property cannot be empty.")));
	}

	if (CharacterCreatorOutfit->Meshes.IsEmpty())
	{
		AssetFails(InAsset, FText::FromString(FString("Meshes property cannot be empty.")));
	}

	bool bMaterialVariants = true;
	for (FCharacterCreatorMaterialVariant MaterialVariant : CharacterCreatorOutfit->MaterialVariants)
	{
		if (MaterialVariant.Materials.IsEmpty())
		{
			bMaterialVariants = false;
		}
	}
	if (!bMaterialVariants)
	{
		AssetFails(InAsset, FText::FromString(FString("Each MaterialVariant has to have at least one material asset")));
	}

	if (GetValidationResult() != EDataValidationResult::Invalid)
	{
		AssetPasses(InAsset);
	}

	return GetValidationResult();
}

