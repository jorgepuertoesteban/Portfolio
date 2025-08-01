// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CharacterCreatorOutfitModifier.h"
#include "CCOM_MorphTarget.generated.h"


UCLASS(EditInlineNew)
class CHARACTERCREATION_API UCCOM_MorphTarget : public UCharacterCreatorOutfitModifier
{
	GENERATED_BODY()

public:

	virtual void PerformModification(UCharacterCreatorComponent* CharacterCreatorComponent, USkeletalMeshComponent* SkeletalMeshComponent, const UCharacterCreatorOutfit* Outfit, const UCharacterCreator* CharacterCreator) const override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MorphName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value = 1.0f;

};
