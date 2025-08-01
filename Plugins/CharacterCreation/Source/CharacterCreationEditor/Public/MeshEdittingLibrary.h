// Created by Jorge Puerto. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MeshDescription.h"
#include "Animation/MorphTarget.h"
#include "Engine/StaticMesh.h"
#include "SkeletalRenderPublic.h"
#include "Engine/EngineTypes.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "MeshEdittingLibrary.generated.h"

class USkeletalMesh;
class UBAMorphTarget;
struct FMeshDescription;


class FMeshMorpherMorphTargetInfo
{
public:
	FName Name;
	float Weight;
	bool bRemove;
	int32 NumberOfVerts;

	/** Static function for creating a new item, but ensures that you can only have a TSharedRef to one */
	static TSharedRef<FMeshMorpherMorphTargetInfo> Make(const FName& Source, int32 InNumberOfVerts)
	{
		return MakeShareable(new FMeshMorpherMorphTargetInfo(Source, InNumberOfVerts));
	}

protected:
	/** Hidden constructor, always use Make above */
	FMeshMorpherMorphTargetInfo(const FName& InSource, int32 InNumberOfVerts)
		: Name(InSource)
		, Weight(0)
		, bRemove(false)
		, NumberOfVerts(InNumberOfVerts)
	{}

	/** Hidden constructor, always use Make above */
	FMeshMorpherMorphTargetInfo() {}
};



UCLASS()
class CHARACTERCREATIONEDITOR_API UMeshEdittingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	
public:
	static void NotifyMessage(const FString& Message);
	static bool CopySkeletalMeshMaterialsToStaticMesh(USkeletalMesh* SkeletalMesh, UStaticMesh* StaticMesh);
	static bool CopySkeletalMeshMaterials(USkeletalMesh* SkeletalMesh, TArray<FStaticMaterial>& StaticMaterials);
	static bool CopySkeletalMeshMaterials(USkeletalMesh* SkeletalMesh, TMap<int32, FName>& StaticMaterials);
	static bool CopySkeletalMeshMaterials(USkeletalMesh* SkeletalMesh, TMap<FName, int32>& StaticMaterials);
	static bool SetStaticMesh(UStaticMesh* StaticMesh, const FMeshDescription& MeshDescription);
	static bool SkeletalMeshToDynamicMesh(USkeletalMesh* SkeletalMesh, UE::Geometry::FDynamicMesh3& IdenticalDynamicMesh, UE::Geometry::FDynamicMesh3* WeldedDynamicMesh = NULL, const TArray<FFinalSkinVertex>& FinalVertices = TArray<FFinalSkinVertex>(), int32 LOD = 0, bool bUseRenderData = true);
	static bool SkeletalMeshToDynamicMesh_RenderData(USkeletalMesh* SkeletalMesh, UE::Geometry::FDynamicMesh3& IdenticalDynamicMesh, UE::Geometry::FDynamicMesh3* WeldedDynamicMesh = NULL, const TArray<FFinalSkinVertex>& FinalVertices = TArray<FFinalSkinVertex>(), int32 LOD = 0);
	static void ApplyDeltasToDynamicMesh(const TArray<FMorphTargetDelta>& Deltas, UE::Geometry::FDynamicMesh3& DynamicMesh);
	static void ApplyDeltasToDynamicMesh(UE::Geometry::FDynamicMesh3& SourceDynamicMesh, const TArray<FMorphTargetDelta>& Deltas, UE::Geometry::FDynamicMesh3& DynamicMesh);
	static void GetMorphDeltas(const UE::Geometry::FDynamicMesh3& Original, const UE::Geometry::FDynamicMesh3& Changed, TArray<FMorphTargetDelta>& Deltas);
	static void ApplyChangesToMorphTarget(USkeletalMesh* Mesh, const UE::Geometry::FDynamicMesh3& DynamicMesh, FString MorphTargetName, const UE::Geometry::FDynamicMesh3& Original, const UE::Geometry::FDynamicMesh3& Changed);
	static void RenameMorphTargetInImportData(USkeletalMesh* Mesh, FString NewName, FString OriginalName, bool bInvalidateRenderData = true);
	static void RemoveMorphTargetsFromImportData(USkeletalMesh* Mesh, const TArray<FString>& MorphTargets, bool bInvalidateRenderData = true);
	static void ApplyMorphTargetToImportData(USkeletalMesh* Mesh, FString MorphTargetName, const TArray<FMorphTargetDelta>& Deltas, bool bInvalidateRenderData = true);
	static void ApplyMorphTargetToImportData(USkeletalMesh* Mesh, FString MorphTargetName, const TArray<FMorphTargetDelta>& Deltas, int32 LOD);
	static void GetMorphTargetDeltas(USkeletalMesh* Mesh, FString MorphTargetName, TArray<FMorphTargetDelta>& Deltas, int32 LOD = 0);
	static void GetMorphTargetDeltas(USkeletalMesh* Mesh, UMorphTarget* MorphTarget, TArray<FMorphTargetDelta>& Deltas, int32 LOD = 0);
	static void GetMorphTargetNames(USkeletalMesh* Mesh, TArray<FString>& MorphTargets);
	static void SetEnableBuildData(USkeletalMesh* Mesh, bool NewValue);
	static void ApplyMorphTargetToLODs(USkeletalMesh* Mesh, FString MorphTargetName, const TArray<FMorphTargetDelta>& Deltas);
	static bool ApplyMorphTargetsToSkeletalMesh(USkeletalMesh* SkeletalMesh, const TArray< TSharedPtr<FMeshMorpherMorphTargetInfo> >& MorphTargets);
	static void SaveSkeletalMesh(USkeletalMesh* Mesh);
	static void ExportMorphTargetToStaticMesh(FString MorphTargetName, const FMeshDescription& MeshDescription, const TArray<FStaticMaterial>& StaticMaterials);
	static void ExportMorphTargetToStandAlone(USkeletalMesh* Mesh, FString MorphTargetName);
	static bool ApplyMorphTargetToLOD(USkeletalMesh* SkeletalMesh, const TArray<FMorphTargetDelta>& Deltas, int32 SourceLOD, int32 DestinationLOD, TArray<FMorphTargetDelta>& OutDeltas);
	static UMorphTarget* FindMorphTarget(USkeletalMesh* Mesh, FString MorphTargetName);
	static void CreateMorphTargetObj(USkeletalMesh* Mesh, FString MorphTargetName, bool bInvalidateRenderData = true);
	static void CreateMorphTarget(USkeletalMesh* Mesh, FString MorphTargetName);
	static bool MergeMorphTargets(USkeletalMesh* SkeletalMesh, const TArray<FString>& MorphTargets, TArray<FMorphTargetDelta>& OutDeltas);
	static bool InvertMorphTarget(USkeletalMesh* SkeletalMesh, const FString& MorphTarget, TArray<FMorphTargetDelta>& OutDeltas);
	static bool CopyMorphTarget(USkeletalMesh* SkeletalMesh, const TArray<FString>& MorphTargets, USkeletalMesh* TargetSkeletalMesh, TArray<TArray<FMorphTargetDelta>>& OutDeltas, float Threshold = 20.f, float Multiplier = 1.f, int32 SmoothIterations = 0, float SmoothStrength = 0.8f);
	static void ApplySourceDeltasToDynamicMesh(const UE::Geometry::FDynamicMesh3& SourceDynamicMesh, const UE::Geometry::FDynamicMesh3& DynamicMesh, const TArray<FMorphTargetDelta>& SourceDeltas, TArray<FMorphTargetDelta>& OutDeltas, float Threshold = 1.f, float Multiplier = 1.f, int32 SmoothIterations = 0, float SmoothStrength = 1.0f, bool bCheckIdentical = true);
	static bool ImportMorphTarget(USkeletalMesh* SkeletalMesh, const TArray<UBAMorphTarget*>& MorphTargets, USkeletalMesh* TargetSkeletalMesh, TArray<TArray<FMorphTargetDelta>>& OutDeltas, float Threshold = 20.f, float Multiplier = 1.f, int32 SmoothIterations = 0, float SmoothStrength = 0.8f);
	static bool CreateMorphTargetFromPose(USkeletalMesh* SkeletalMesh, const UE::Geometry::FDynamicMesh3& PoseDynamicMesh, TArray<FMorphTargetDelta>& OutDeltas);
	static bool CreateMorphTargetFromMesh(USkeletalMesh* SkeletalMesh, USkeletalMesh* SourceSkeletalMesh, TArray<FMorphTargetDelta>& OutDeltas, float Threshold = 20.f, float Multiplier = 1.f, int32 SmoothIterations = 0, float SmoothStrength = 0.8f);
	static void ReadObj(const FString& Path, UE::Geometry::FDynamicMesh3& Mesh, bool bYUp = false);
	static bool CreateMorphTargetFromObj(USkeletalMesh* SkeletalMesh, const FFilePath& BaseObjPath, const FFilePath& MorphedObjPath, TArray<FMorphTargetDelta>& OutDeltas, float Threshold = 1.f, float Multiplier = 1.f, int32 SmoothIterations = 0, float SmoothStrength = 1.0f, bool bYUp = false);
};