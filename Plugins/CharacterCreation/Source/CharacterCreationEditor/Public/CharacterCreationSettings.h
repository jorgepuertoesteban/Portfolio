// Copyright 2020-2021 SC Pug Life Studio S.R.L. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "UObject/SoftObjectPtr.h"
#include "Materials/MaterialInterface.h"
#include "CharacterCreationSettings.generated.h"

UCLASS(config = CharacterCreation)
class CHARACTERCREATIONEDITOR_API UCharacterCreationSettings : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(config, EditAnywhere, Category = "Preview|Materials")
	TSoftObjectPtr<UMaterialInterface> MeshFocusMaterial;

	UPROPERTY(EditAnywhere, Category = "UI|LOD")
	bool bRemapMorphTargets = true;

	UPROPERTY(config, EditAnywhere, Meta = (Latent, DisplayName = "Use UV Projection for LODs"), Category = "UI|LOD")
	bool bUseUVProjectionForLODs = true;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", UIMin = "0", HideEditConditionToggle, EditConditionHides, EditCondition = "bUseUVProjectionForLODs"), Category = "UI|LOD")
	float Threshold = 20.f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", UIMin = "0", HideEditConditionToggle, EditConditionHides, EditCondition = "bUseUVProjectionForLODs"), Category = "UI|LOD")
	int32 SmoothIterations = 3;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", UIMin = "0", HideEditConditionToggle, EditConditionHides, EditCondition = "bUseUVProjectionForLODs"), Category = "UI|LOD")
	float SmoothStrength = 0.6f;
};