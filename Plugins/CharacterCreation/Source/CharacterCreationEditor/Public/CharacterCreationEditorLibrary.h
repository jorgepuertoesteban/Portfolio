// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Rendering/SkeletalMeshModel.h"
#include "CharacterCreationEditorLibrary.generated.h"

class UDebugSkelMeshComponent;
class UObject;
class USkeleton;
class USkeletalMesh;

DECLARE_LOG_CATEGORY_EXTERN(ReferencePoseEditorLog, All, All);

/**
 *
 */
UCLASS(BlueprintType)
class CHARACTERCREATIONEDITOR_API UCharacterCreationEditorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static const FText TransactionNameRefSkel;

	UFUNCTION(BlueprintCallable, Category = "CharacterCreation")
	static void UpdateSkelMeshReferencePoseFromCurrent(UDebugSkelMeshComponent* SkeletalComponent, bool bUpdateRefSkel, bool bUpdateVertexPositions, bool bRotations, bool bLocations, bool bScale);

	UFUNCTION(BlueprintCallable, Category = "CharacterCreation")
	static void UpdateSkelMeshReferencePoseFromSource(UDebugSkelMeshComponent* SkeletalComponent, UObject* MeshOrSkelSource, bool bUpdateRefSkel, bool bUpdateVertexPositions, bool bRotations, bool bLocations, bool bScale);

	UFUNCTION(BlueprintCallable, Category = "CharacterCreation")
	static void DeleteExtraBones(UDebugSkelMeshComponent* SkeletalComponent, UObject* MeshOrSkelSource, bool bUpdateRefSkel, bool bUpdateVertexPositions, bool bRotations, bool bLocations, bool bScale);

	UFUNCTION(BlueprintCallable, Category = "CharacterCreation")
	static void CopyBonesLocation(UDebugSkelMeshComponent* SkeletalComponent, UObject* MeshOrSkelSource);
	/*
	UFUNCTION(BlueprintCallable, Category = "CharacterCreation")
	static void CopyBonesToPreview(UDebugSkelMeshComponent* SkeletalComponent, UObject* MeshOrSkelSource, bool bUpdateRefSkel, bool bUpdateVertexPositions, bool bRotations, bool bLocations, bool bScale);

	UFUNCTION(BlueprintCallable, Category = "CharacterCreation")
	static void SaveCurrentReferencePose(UDebugSkelMeshComponent* SkeletalComponent);
	*/

	UFUNCTION(BlueprintCallable, Category = "CharacterCreation")
	static void ResetModifiedBones(UDebugSkelMeshComponent* SkeletalComponent);

	UFUNCTION(BlueprintCallable, Category = "CharacterCreation")
	static void BakeCurrentGeometry(UDebugSkelMeshComponent* SkeletalComponent);

	UFUNCTION(BlueprintCallable, Category = "CharacterCreation")
	static bool MergeBonesToBoneTreeDeletingIntermediate(USkeleton* const Skeleton, USkeletalMesh* const InSkeletalMesh, const TArray<int32>& RequiredRefBones);

private:
	
	static void LogSekeleton(const USkeleton* SelectedSkeleton);
	static void LogSekeletalMesh(const USkeletalMesh* EditingSkeletalMesh);
	static void LogReferenceSkeleton(const FReferenceSkeleton& EditingRefSkeleton);
};
