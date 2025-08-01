// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorValidatorBase.h"
#include "CharacterCreatorOutfitValidator.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONEDITOR_API UCharacterCreatorOutfitValidator : public UEditorValidatorBase
{
	GENERATED_BODY()

protected:
	//~UEditorValidatorBase interface
	virtual bool CanValidateAsset_Implementation(UObject* InAsset) const override;
	virtual EDataValidationResult ValidateLoadedAsset_Implementation(UObject* InAsset, TArray<FText>& ValidationErrors) override;
	//~End of UEditorValidatorBase interface
};
