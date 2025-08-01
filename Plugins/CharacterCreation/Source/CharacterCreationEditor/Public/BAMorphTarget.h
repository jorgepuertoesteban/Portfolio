// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "PackedNormal.h"
#include "UObject/EditorObjectVersion.h"
#include "UObject/FortniteMainBranchObjectVersion.h"
#include "Animation/MorphTarget.h"
#include "BAMorphTarget.generated.h"

UCLASS(hidecategories = Object, BlueprintType)
class CHARACTERCREATIONEDITOR_API UBAMorphTarget : public UObject
{
	GENERATED_BODY()

public:

	/** USkeletalMesh that this vertex animation works on. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Standalone Morph Target")
	class USkeletalMesh* BaseSkelMesh;
	/** morph mesh vertex data for each LOD */
	TArray<FMorphTargetLODModel> MorphLODModels;

public:

	/** Get Morphtarget Delta array for the given input Index */
	FMorphTargetDelta* GetMorphTargetDelta(int32 LODIndex, int32& OutNumDeltas);
	bool HasDataForLOD(int32 LODIndex);
	/** return true if this morphtarget contains valid vertices */
	bool HasValidData() const;

#if WITH_EDITOR
	/** Populates the given morph target LOD model with the provided deltas */
	void PopulateDeltas(const TArray<FMorphTargetDelta>& Deltas, const int32 LODIndex, const TArray<struct FSkelMeshSection>& Sections, const bool bCompareNormal = false, const bool bGeneratedByReductionSetting = false, const float PositionThreshold = THRESH_POINTS_ARE_NEAR);
	/** Remove empty LODModels */
	void RemoveEmptyMorphTargets();
#endif // WITH_EDITOR

public:

	//~ UObject interface

	virtual void Serialize(FArchive& Ar) override;
	virtual void GetResourceSizeEx(FResourceSizeEx& CumulativeResourceSize) override;
	virtual void PostLoad() override;
};