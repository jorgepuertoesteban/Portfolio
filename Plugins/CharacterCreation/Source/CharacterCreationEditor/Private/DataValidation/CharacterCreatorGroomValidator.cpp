// Created by Jorge Puerto. All Rights Reserved.


#include "DataValidation/CharacterCreatorGroomValidator.h"
#include "CharacterCreatorGroom.h"

bool UCharacterCreatorGroomValidator::CanValidateAsset_Implementation(UObject* InAsset) const
{
	return (InAsset != nullptr) && (UCharacterCreatorGroom::StaticClass() == InAsset->GetClass());
}

EDataValidationResult UCharacterCreatorGroomValidator::ValidateLoadedAsset_Implementation(UObject* InAsset, TArray<FText>& ValidationErrors)
{
	check(UCharacterCreatorGroom::StaticClass() == InAsset->GetClass());

	UCharacterCreatorGroom* CharacterCreatorGroom = Cast<UCharacterCreatorGroom>(InAsset);

	if (CharacterCreatorGroom == nullptr)
	{
		return EDataValidationResult::Valid;
	}

	if (CharacterCreatorGroom->Slot == nullptr)
	{
		AssetFails(InAsset, FText::FromString(FString("Slot property cannot be empty")));
	}

	if (CharacterCreatorGroom->SlotToAttach == nullptr)
	{
		AssetFails(InAsset, FText::FromString(FString("SlotToAttach property cannot be empty")));
	}

	if (CharacterCreatorGroom->GroomAsset.IsNull())
	{
		AssetFails(InAsset, FText::FromString(FString("GroomAsset property cannot be empty")));
	}

	if (CharacterCreatorGroom->BindingsMap.IsEmpty())
	{
		AssetFails(InAsset, FText::FromString(FString("BindingsMap property cannot be empty")));
	}
	TArray<TSoftObjectPtr<UGroomBindingAsset>> BindingsMapValueArray;
	CharacterCreatorGroom->BindingsMap.GenerateValueArray(BindingsMapValueArray);

	bool bEachBindingIsSet = true;
	for (TSoftObjectPtr<UGroomBindingAsset> GroomBindingAsset : BindingsMapValueArray)
	{
		if (GroomBindingAsset.IsNull())
		{
			bEachBindingIsSet = false;
		}
	}
	if (!bEachBindingIsSet)
	{
		AssetFails(InAsset, FText::FromString(FString("Each item inside BindingsMap has to have value")));
	}

	if (GetValidationResult() != EDataValidationResult::Invalid)
	{
		AssetPasses(InAsset);
	}

	return GetValidationResult();
}
