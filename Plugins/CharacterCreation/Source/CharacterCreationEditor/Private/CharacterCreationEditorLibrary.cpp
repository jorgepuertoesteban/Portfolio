// Created by Jorge Puerto. All Rights Reserved.


#include "CharacterCreationEditorLibrary.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/DebugSkelMeshComponent.h"
#include "AnimPreviewInstance.h"
#include "Rendering/SkeletalMeshModel.h"
#include "SkeletalRenderPublic.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "CharacterCreationEditorLog.h"
#include "Animation/Skeleton.h"
#include "Rendering/SkeletalMeshLODModel.h"

#define LOCTEXT_NAMESPACE "CharacterCreationEditorLibrary"

const FText UCharacterCreationEditorLibrary::TransactionNameRefSkel = FText::FromString("Update Reference Pose Ref Skeleton");

void LoadImportDataHelper
(
	USkeletalMesh* Mesh,
	FSkeletalMeshModel*& OutImportedModel,
	FSkeletalMeshLODModel*& OutLod0Model,
	FSkeletalMeshImportData& OutData,
	TMap<FString, TArray<FMorphTargetDelta>>& OutReductionLODMorphTargetData,
	bool& OutLoadedReductionData,
	bool& OutbIsLod0Reduced
)
{

	OutImportedModel = Mesh->GetImportedModel();
	OutLod0Model = &(OutImportedModel->LODModels[0]);
	OutbIsLod0Reduced = Mesh->IsReductionActive(0);

	// if lod 0 has been reduced its a special case
	// we need to modify the original mesh data and then rebuild the lods
	//if (
	//	OutbIsLod0Reduced
	//	&& OutImportedModel->OriginalReductionSourceMeshData_DEPRECATED.Num()
	//	&& !OutImportedModel->OriginalReductionSourceMeshData_DEPRECATED[0]->IsEmpty()
	//	)
	//{
	//	OutImportedModel->OriginalReductionSourceMeshData_DEPRECATED[0]->LoadReductionData(*OutLod0Model, OutReductionLODMorphTargetData, Mesh);
	//	OutLoadedReductionData = true;
	//}
	//else
	//{
	//	OutLoadedReductionData = false;
	//}


	////handle legacy assets imported before current engine version, not sure if this is needed
	//if (
	//	!OutLod0Model->bIsBuildDataAvailable &&
	//	!OutLod0Model->bIsRawSkeletalMeshBulkDataEmpty &&
	//	!OutLod0Model->RawSkeletalMeshBulkData_DEPRECATED.IsEmpty()
	//	)
	//{
	//	OutLod0Model->RawSkeletalMeshBulkData_DEPRECATED.LoadRawMesh(OutData);
	//}
	//else if (OutLod0Model->bIsBuildDataAvailable)
	//{
	//	Mesh->LoadLODImportedData(0, OutData);
	//}
}

void BakeVertices(
	USkeletalMesh* TargetSkeletalMesh,
	const TArray<FFinalSkinVertex>& PosedVertexPositions,
	const FMatrix& WorldToRoot,
	const FMatrix& ComponentToWorld
)
{
	//attempt to load import/reduction data
	FSkeletalMeshModel* ImportedModel = nullptr;
	FSkeletalMeshLODModel* ImportedModelLod0 = nullptr;
	FSkeletalMeshImportData RawLod0ImportDataMesh;
	TMap<FString, TArray<FMorphTargetDelta>> ReductionLODMorphTargetData;
	bool bModifiedReductionData;
	bool bIsLod0Reduced;

	LoadImportDataHelper(
		TargetSkeletalMesh,
		ImportedModel,
		ImportedModelLod0,
		RawLod0ImportDataMesh,
		ReductionLODMorphTargetData,
		bModifiedReductionData,
		bIsLod0Reduced
	);

	TArray<FVector> RawPositions;
	for (int32 VertIndex = 0; VertIndex < PosedVertexPositions.Num(); ++VertIndex)
	{
		//RawPositions.Add(ComponentToWorld.TransformPosition(PosedVertexPositions[VertIndex].Position));
	}
	for (int32 RenderVertIndex = 0; RenderVertIndex < RawPositions.Num(); RenderVertIndex++)
	{
		int32 ImportVertIdx = ImportedModelLod0->MeshToImportVertexMap[RenderVertIndex];
		//RawLod0ImportDataMesh.Points[ImportVertIdx] = RawPositions[RenderVertIndex];
	}

	//save LOD import data, and make new guid to force DDC re-creation
	FMeshDescription MeshDescription;

	if (RawLod0ImportDataMesh.GetMeshDescription(nullptr, &TargetSkeletalMesh->GetLODInfo(0)->BuildSettings, MeshDescription))
	{
		TargetSkeletalMesh->CreateMeshDescription(0, MoveTemp(MeshDescription));
		TargetSkeletalMesh->CommitMeshDescription(0);
	}

	TargetSkeletalMesh->InvalidateDeriveDataCacheGUID();
	TargetSkeletalMesh->Build();

}

FTransform GetTransformToUse(
	const FTransform& OldTransform,
	const FTransform& NewTransform,
	const bool bRotations,
	const bool bLocations,
	const bool bScale
)
{
	FTransform TransformToUse = FTransform::Identity;
	if (bRotations)
	{
		TransformToUse.SetRotation(NewTransform.GetRotation());
	}
	else
	{
		TransformToUse.SetRotation(OldTransform.GetRotation());
	}

	if (bLocations)
	{
		TransformToUse.SetLocation(NewTransform.GetLocation());
	}
	else
	{

		TransformToUse.SetLocation(OldTransform.GetLocation());
	}
	if (bScale)
	{
		TransformToUse.CopyScale3D(NewTransform);
	}
	else
	{
		TransformToUse.CopyScale3D(OldTransform);
	}
	return TransformToUse;
}

void UCharacterCreationEditorLibrary::UpdateSkelMeshReferencePoseFromCurrent(UDebugSkelMeshComponent* Component, bool bUpdateRefSkel, bool bUpdateVertexPositions, bool bRotations, bool bLocations, bool bScale)
{
	if (!Component)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because UDebugSkelMeshComponent is null."));
		//TODO: Create alert
		return;
	}

	USkeletalMesh* TargetSkeletalMesh = Cast<USkeletalMesh>(Component->GetSkinnedAsset());
	if (!TargetSkeletalMesh)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because The SkeletalMesh of the SkeletalMeshComponent is null."));
		//TODO: Create alert
		return;
	}


	if (bUpdateRefSkel)
	{
		FScopedTransaction Transaction(LOCTEXT("UpdateSkelMeshReferencePoseFromCurrent", "UpdateSkelMeshReferencePoseFromCurrent"));

		TargetSkeletalMesh->Modify();
		{
			USkeleton* Skeleton = TargetSkeletalMesh->GetSkeleton();
			FReferenceSkeleton& CurrentMeshRefSkeleton = TargetSkeletalMesh->GetRefSkeleton();
			FReferenceSkeletonModifier RefSkeletonModifier(CurrentMeshRefSkeleton, Skeleton);
			// get current preview mesh transform
			TArray<FTransform> CurrentPose = Component->GetComponentSpaceTransforms();

			// we want to only apply these to raw ref pose transform
			const TArray<FTransform>& MeshRefPose = CurrentMeshRefSkeleton.GetRawRefBonePose();

			// if this isnt true... idk
			check(MeshRefPose.Num() <= CurrentPose.Num());

			for (int32 MeshRefPoseIdx = 0; MeshRefPoseIdx < MeshRefPose.Num(); ++MeshRefPoseIdx)
			{
				FTransform NewTransform;
				FTransform OldTransform = MeshRefPose[MeshRefPoseIdx];

				//root has no parent
				if (MeshRefPoseIdx == 0)
				{
					NewTransform = CurrentPose[MeshRefPoseIdx];
				}
				else
				{
					const int32 ParentIndex = CurrentMeshRefSkeleton.GetParentIndex(MeshRefPoseIdx);
					NewTransform = CurrentPose[MeshRefPoseIdx].GetRelativeTransform(CurrentPose[ParentIndex]);
				}

				if (!NewTransform.Equals(OldTransform))
				{
					FTransform TransformToUse = GetTransformToUse(
						OldTransform,
						NewTransform,
						bRotations,
						bLocations,
						bScale
					);
					RefSkeletonModifier.UpdateRefPoseTransform(MeshRefPoseIdx, TransformToUse);
				}
			}
		}
	}

	if (bUpdateVertexPositions)
	{
		FMatrix WorldToRoot = Component->GetComponentToWorld().ToMatrixWithScale().Inverse();
		FMatrix ComponentToWorld = Component->GetComponentTransform().ToMatrixWithScale() * WorldToRoot;
		TArray<FFinalSkinVertex> PosedVerts;
		Component->GetCPUSkinnedVertices(PosedVerts, 0);
		BakeVertices(TargetSkeletalMesh, PosedVerts, WorldToRoot, ComponentToWorld);
	}

	//we have to empty them to force them to be recalculated
	TargetSkeletalMesh->GetRefBasesInvMatrix().Empty();
	TargetSkeletalMesh->CalculateInvRefMatrices();

	// reset transforms in persona
	if (Component && Component->PreviewInstance)
	{
		Component->PreviewInstance->ResetModifiedBone();
	}

	//dirty it
	TargetSkeletalMesh->PostEditChange();
	TargetSkeletalMesh->MarkPackageDirty();
}

void UCharacterCreationEditorLibrary::UpdateSkelMeshReferencePoseFromSource(UDebugSkelMeshComponent* Component, UObject* MeshOrSkelSource, bool bUpdateRefSkel, bool bUpdateVertexPositions, bool bRotations, bool bLocations, bool bScale)
{
	check(Component && MeshOrSkelSource);
	check(MeshOrSkelSource->IsA(USkeletalMesh::StaticClass()) || MeshOrSkelSource->IsA(USkeleton::StaticClass()));

	USkeletalMesh* TargetSkeletalMesh = Cast<USkeletalMesh>(Component->GetSkinnedAsset());

	if (TargetSkeletalMesh)
	{
		FReferenceSkeleton& TargetMeshRefSkeleton = TargetSkeletalMesh->GetRefSkeleton();
		const FReferenceSkeleton& SourcePoseRefSkeleton = (
			MeshOrSkelSource->IsA(USkeletalMesh::StaticClass()) ?
			Cast<USkeletalMesh>(MeshOrSkelSource)->GetRefSkeleton() :
			Cast<USkeleton>(MeshOrSkelSource)->GetReferenceSkeleton()
			);

		if (bUpdateRefSkel)
		{
			FScopedTransaction Transaction(LOCTEXT("UpdateSkelMeshReferencePoseFromSource", "UpdateSkelMeshReferencePoseFromSource"));
			TargetSkeletalMesh->Modify();
			{
				FReferenceSkeletonModifier RefSkeletonModifier(TargetMeshRefSkeleton, TargetSkeletalMesh->GetSkeleton());

				TArray<FTransform> TargetMeshPose = TargetMeshRefSkeleton.GetRawRefBonePose();
				TArray<FTransform> PoseToCopy = SourcePoseRefSkeleton.GetRawRefBonePose();
				for (int32 TargetIdx = 0; TargetIdx < TargetMeshPose.Num(); ++TargetIdx)
				{
					FTransform NewTransform;
					FTransform OldTransform = TargetMeshPose[TargetIdx];
					//root has no parent
					if (TargetIdx == 0)
					{
						NewTransform = PoseToCopy[TargetIdx];
					}
					else
					{
						const FName TargetName = TargetMeshRefSkeleton.GetBoneName(TargetIdx);
						const int32 SourceIdx = SourcePoseRefSkeleton.FindBoneIndex(TargetName);
						if (SourceIdx == INDEX_NONE)
						{
							continue;
						}
						NewTransform = PoseToCopy[SourceIdx];
					}

					if (!NewTransform.Equals(OldTransform))
					{
						FTransform TransformToUse = GetTransformToUse(
							OldTransform,
							NewTransform,
							bRotations,
							bLocations,
							bScale
						);
						RefSkeletonModifier.UpdateRefPoseTransform(TargetIdx, TransformToUse);
					}
				}
			}
		}

		if (bUpdateVertexPositions)
		{
			FMatrix WorldToRoot = Component->GetComponentToWorld().ToMatrixWithScale().Inverse();
			FMatrix ComponentToWorld = Component->GetComponentTransform().ToMatrixWithScale() * WorldToRoot;
			TArray<FFinalSkinVertex> PosedVerts;
			Component->GetCPUSkinnedVertices(PosedVerts, 0);
			BakeVertices(TargetSkeletalMesh, PosedVerts, WorldToRoot, ComponentToWorld);
		}

		//we have to empty them to force them to be recalculated
		TargetSkeletalMesh->GetRefBasesInvMatrix().Empty();
		TargetSkeletalMesh->CalculateInvRefMatrices();

		// reset transforms in persona
		if (Component && Component->PreviewInstance)
		{
			Component->PreviewInstance->ResetModifiedBone();
		}

		//dirty itTargetSkeletalMesh
		TargetSkeletalMesh->PostEditChange();
		TargetSkeletalMesh->MarkPackageDirty();
	}
}


void UCharacterCreationEditorLibrary::DeleteExtraBones(UDebugSkelMeshComponent* Component, UObject* MeshOrSkelSource, bool bUpdateRefSkel, bool bUpdateVertexPositions, bool bRotations, bool bLocations, bool bScale)
{
	if (!Component)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because UDebugSkelMeshComponent is null."));
		//TODO: Create alert
		return;
	}

	USkeletalMesh* TargetSkeletalMesh = Cast<USkeletalMesh>(Component->GetSkinnedAsset());
	if (!TargetSkeletalMesh)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because The SkeletalMesh of the SkeletalMeshComponent is null."));
		//TODO: Create alert
		return;
	}

	if (bUpdateRefSkel)
	{
		FScopedTransaction Transaction(LOCTEXT("DeleteExtraBones", "DeleteExtraBones"));

		TargetSkeletalMesh->Modify();
		{
			USkeleton* Skeleton = TargetSkeletalMesh->GetSkeleton();
			FReferenceSkeleton& CurrentMeshRefSkeleton = TargetSkeletalMesh->GetRefSkeleton();
			FReferenceSkeletonModifier RefSkeletonModifier(CurrentMeshRefSkeleton, Skeleton);
			// get current preview mesh transform
			TArray<FTransform> CurrentPose = Component->GetComponentSpaceTransforms();

			// we want to only apply these to raw ref pose transform
			const TArray<FTransform>& MeshRefPose = CurrentMeshRefSkeleton.GetRawRefBonePose();

			// if this isnt true... idk
			check(MeshRefPose.Num() <= CurrentPose.Num());

			for (int32 MeshRefPoseIdx = 0; MeshRefPoseIdx < MeshRefPose.Num(); ++MeshRefPoseIdx)
			{
				FTransform NewTransform;
				FTransform OldTransform = MeshRefPose[MeshRefPoseIdx];

				//root has no parent
				if (MeshRefPoseIdx == 0)
				{
					NewTransform = CurrentPose[MeshRefPoseIdx];
				}
				else
				{
					const int32 ParentIndex = CurrentMeshRefSkeleton.GetParentIndex(MeshRefPoseIdx);
					NewTransform = CurrentPose[MeshRefPoseIdx].GetRelativeTransform(CurrentPose[ParentIndex]);
				}

				if (!NewTransform.Equals(OldTransform))
				{
					FTransform TransformToUse = GetTransformToUse(
						OldTransform,
						NewTransform,
						bRotations,
						bLocations,
						bScale
					);
					RefSkeletonModifier.UpdateRefPoseTransform(MeshRefPoseIdx, TransformToUse);
				}
			}
		}
	}

	if (bUpdateVertexPositions)
	{
		FMatrix WorldToRoot = Component->GetComponentToWorld().ToMatrixWithScale().Inverse();
		FMatrix ComponentToWorld = Component->GetComponentTransform().ToMatrixWithScale() * WorldToRoot;
		TArray<FFinalSkinVertex> PosedVerts;
		Component->GetCPUSkinnedVertices(PosedVerts, 0);
		BakeVertices(TargetSkeletalMesh, PosedVerts, WorldToRoot, ComponentToWorld);
	}

	//we have to empty them to force them to be recalculated
	TargetSkeletalMesh->GetRefBasesInvMatrix().Empty();
	TargetSkeletalMesh->CalculateInvRefMatrices();

	// reset transforms in persona
	if (Component && Component->PreviewInstance)
	{
		Component->PreviewInstance->ResetModifiedBone();
	}

	//dirty it
	TargetSkeletalMesh->PostEditChange();
	TargetSkeletalMesh->MarkPackageDirty();
}




void UCharacterCreationEditorLibrary::CopyBonesLocation(UDebugSkelMeshComponent* DebugSkeletalComponent, UObject* SelectedObject)
{
#pragma region CheckInputValues
	if (!DebugSkeletalComponent)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because UDebugSkelMeshComponent is null."));
		//TODO: Create alert
		return;
	}

	USkeletalMesh* EdittingSkeletalMesh = Cast<USkeletalMesh>(DebugSkeletalComponent->GetSkinnedAsset());
	if (!EdittingSkeletalMesh)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because The SkeletalMesh of the SkeletalMeshComponent is null."));
		return;
	}
	LogSekeletalMesh(EdittingSkeletalMesh);//Lets do some Logging
	EdittingSkeletalMesh->InvalidateDeriveDataCacheGUID();

	USkeleton* SelectedSkeleton = SelectedObject && SelectedObject->IsA<USkeletalMesh>() ? Cast<USkeletalMesh>(SelectedObject)->GetSkeleton() : Cast<USkeleton>(SelectedObject);
	if (!SelectedSkeleton)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("The Selected Object is Incompatible or null."));
		return;
	}
#pragma endregion

	//Start transaction for Undo/Redo
	FScopedTransaction Transaction(LOCTEXT("CopyBonesLocation", "CopyBonesLocation"));

	TArray<FFinalSkinVertex> PosedVertsToTest;
	DebugSkeletalComponent->GetCPUSkinnedVertices(PosedVertsToTest, 0);

	//From MeshMorpher
	EdittingSkeletalMesh->WaitForPendingInitOrStreaming();
	//End From MeshMorpher

	EdittingSkeletalMesh->Modify();

	FReferenceSkeleton& EdittingRefSkeleton = EdittingSkeletalMesh->GetRefSkeleton();
	const FReferenceSkeleton& SelectedRefSkeleton = SelectedSkeleton->GetReferenceSkeleton();

	FReferenceSkeletonModifier RefSkeletonModifier(EdittingRefSkeleton, EdittingSkeletalMesh->GetSkeleton());

	TArray<FTransform> EdittingMeshPose = EdittingRefSkeleton.GetRawRefBonePose();
	TArray<FTransform> SelectedPoseTransforms = SelectedRefSkeleton.GetRawRefBonePose();

	for (int32 EdittingBoneIndex = 0; EdittingBoneIndex < EdittingMeshPose.Num(); ++EdittingBoneIndex)
	{
		FName const TargetName = EdittingRefSkeleton.GetBoneName(EdittingBoneIndex);
		int32 const SelectedBoneIndex = SelectedRefSkeleton.FindBoneIndex(TargetName);
		if (SelectedBoneIndex != INDEX_NONE)
		{
			RefSkeletonModifier.UpdateRefPoseTransform(EdittingBoneIndex, SelectedPoseTransforms[SelectedBoneIndex]);
		}
	}

	FMatrix const WorldToRoot = DebugSkeletalComponent->GetComponentToWorld().ToMatrixWithScale().Inverse();
	FMatrix const ComponentToWorld = DebugSkeletalComponent->GetComponentTransform().ToMatrixWithScale() * WorldToRoot;
	TArray<FFinalSkinVertex> PosedVerts;
	DebugSkeletalComponent->GetCPUSkinnedVertices(PosedVerts, 0);

	//BakeVertices(EdittingSkeletalMesh, PosedVerts, WorldToRoot, ComponentToWorld);

	//we have to empty them to force them to be recalculated
	EdittingSkeletalMesh->GetRefBasesInvMatrix().Empty();
	EdittingSkeletalMesh->CalculateInvRefMatrices();


	// reset transforms in persona
	DebugSkeletalComponent->PreviewInstance->ResetModifiedBone();
	//From Mesh Morpher
	EdittingSkeletalMesh->InvalidateDeriveDataCacheGUID();
	EdittingSkeletalMesh->InitMorphTargetsAndRebuildRenderData();
	EdittingSkeletalMesh->MarkPackageDirty();
	//End From Mesh Morpher
	//dirty it
	EdittingSkeletalMesh->PostEditChange();
	EdittingSkeletalMesh->MarkPackageDirty();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	EdittingSkeletalMesh->Modify();
	EdittingSkeletalMesh->InvalidateDeriveDataCacheGUID();


	//////////////////////////////////////////////////////////////////////////
	//ApplyMorphTargetToImportData(Mesh, MorphTargetName, Deltas, 0);
	//////////////////////////////////////////////////////////////////////////
	//{
	//	//////////////////////////////////////////////////////////////////////////
	//	UCharacterCreationSettings* Settings = GetMutableDefault<UCharacterCreationSettings>();

	//	if (FSkeletalMeshModel* ImportedModel = EdittingSkeletalMesh->GetImportedModel())
	//	{
	//		int32 const NumberOfLODs = ImportedModel->LODModels.Num();

	//		for (int32 LODIndex = 0; LODIndex < ImportedModel->LODModels.Num(); ++LODIndex)
	//		{
	//			/*UMeshEdittingLibrary::ApplyMorphTargetToImportData(EdittingSkeletalMesh, MorphTargetName, LODDeltas, LODIndex);*/
	//			{
	//				if (ImportedModel->LODModels.IsValidIndex(LODIndex))
	//				{
	//					UE::Geometry::FDynamicMesh3 DynamicMesh;
	//					UMeshEdittingLibrary::SkeletalMeshToDynamicMesh(EdittingSkeletalMesh, DynamicMesh, NULL, TArray<FFinalSkinVertex>(), LODIndex);

	//					FSkeletalMeshLODModel& LODModel = ImportedModel->LODModels[LODIndex];
	//					FSkeletalMeshImportData RawMesh;
	//					EdittingSkeletalMesh->LoadLODImportedData(LODIndex, RawMesh);

	//				}
	//			}
	//		}
	//	}

	//}
	//////////////////////////////////////////////////////////////////////////

	EdittingSkeletalMesh->WaitForPendingInitOrStreaming();

	//FMatrix WorldToRoot = DebugSkeletalComponent->GetComponentToWorld().ToMatrixWithScale().Inverse();
	//FMatrix ComponentToWorld = DebugSkeletalComponent->GetComponentTransform().ToMatrixWithScale() * WorldToRoot;
	//TArray<FFinalSkinVertex> PosedVerts;
	//DebugSkeletalComponent->GetCPUSkinnedVertices(PosedVerts, 0);
	//
	////BakeVertices(TargetSkeletalMesh, PosedVerts, WorldToRoot, ComponentToWorld);


	////attempt to load import/reduction data
	//FSkeletalMeshModel* ImportedModel = nullptr;
	//FSkeletalMeshLODModel* ImportedModelLod0 = nullptr;
	//FSkeletalMeshImportData RawLod0ImportDataMesh;
	//TMap<FString, TArray<FMorphTargetDelta>> ReductionLODMorphTargetData;
	//bool bModifiedReductionData;
	//bool bIsLod0Reduced;

	///*LoadImportDataHelper(
	//	TargetSkeletalMesh,
	//	ImportedModel,
	//	ImportedModelLod0,
	//	RawLod0ImportDataMesh,
	//	ReductionLODMorphTargetData,
	//	bModifiedReductionData,
	//	bIsLod0Reduced
	//);*/

	////////////////////////////////////////////////////////////////////////////

	//ImportedModel = EdittingSkeletalMesh->GetImportedModel();
	//ImportedModelLod0 = &(ImportedModel->LODModels[0]);
	//bIsLod0Reduced = EdittingSkeletalMesh->IsReductionActive(0);

	////////////////////////////////////////////////////////////////////////////


	//TArray<FVector> RawPositions;
	//for (int32 VertIndex = 0; VertIndex < PosedVerts.Num(); ++VertIndex)
	//{
	//	RawPositions.Add(ComponentToWorld.TransformPosition(PosedVerts[VertIndex].Position));
	//}
	//for (int32 RenderVertIndex = 0; RenderVertIndex < RawPositions.Num(); RenderVertIndex++)
	//{
	//	int32 ImportVertIdx = ImportedModelLod0->MeshToImportVertexMap[RenderVertIndex];
	//	RawLod0ImportDataMesh.Points[ImportVertIdx] = RawPositions[RenderVertIndex];
	//}

	////save LOD import data, and make new guid to force DDC re-creation
	//TargetSkeletalMesh->SaveLODImportedData(0, RawLod0ImportDataMesh);
	//TargetSkeletalMesh->InvalidateDeriveDataCacheGUID();
	//TargetSkeletalMesh->Build();




	////////////////////////////////////////////////////////////////////////////
	////ApplyMorphTargetToLODs(Mesh, MorphTargetName, Deltas);
	////////////////////////////////////////////////////////////////////////////
	//EdittingSkeletalMesh->InitMorphTargetsAndRebuildRenderData();
	//EdittingSkeletalMesh->MarkPackageDirty();

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

}
/*
void UCharacterCreationEditorLibrary::CopyBonesToPreview(UDebugSkelMeshComponent* DebugSkeletalComponent, UObject* SelectedObject, bool bUpdateRefSkel, bool bUpdateVertexPositions, bool bRotations, bool bLocations, bool bScale)
{
	if (!DebugSkeletalComponent)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because UDebugSkelMeshComponent is null."));
		//TODO: Create alert
		return;
	}

	USkeletalMesh* EdittingSkeletalMesh = Cast<USkeletalMesh>(DebugSkeletalComponent->GetSkinnedAsset());
	if (!EdittingSkeletalMesh)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because The SkeletalMesh of the SkeletalMeshComponent is null."));
		return;
	}
	LogSekeletalMesh(EdittingSkeletalMesh);//Lets do some Logging

	USkeleton* SelectedSkeleton = SelectedObject && SelectedObject->IsA<USkeletalMesh>() ? Cast<USkeletalMesh>(SelectedObject)->GetSkeleton() : Cast<USkeleton>(SelectedObject);
	if (!SelectedSkeleton)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("The Selected Object is Incompatible or null."));
		return;
	}

	FScopedTransaction Transaction(LOCTEXT("CopyBonesToPreview", "Copy Bones To Preview"));

	DebugSkeletalComponent->PreviewInstance->Modify();
	DebugSkeletalComponent->PreviewInstance->SetFlags(RF_Transactional);

	FReferenceSkeleton& EdittingRefSkeleton = EdittingSkeletalMesh->GetRefSkeleton();
	const FReferenceSkeleton& SelectedRefSkeleton = SelectedSkeleton->GetReferenceSkeleton();
	LogReferenceSkeleton(SelectedRefSkeleton);

	TArray<FTransform> EdittingRefBonePose = EdittingRefSkeleton.GetRawRefBonePose();
	TArray<FTransform> SelectedRefBonePose = SelectedRefSkeleton.GetRawRefBonePose();

	//////////////////////////////////////////////////////////////////////////

	for (int32 SelectedBoneIndex = 0; SelectedBoneIndex < SelectedRefBonePose.Num(); SelectedBoneIndex++)
	{
		FName const BoneName = SelectedRefSkeleton.GetBoneName(SelectedBoneIndex);
		int32 const DebugBoneIndex = DebugSkeletalComponent->GetBoneIndex(BoneName);

		if (DebugBoneIndex != INDEX_NONE)
		{
			const FTransform& SelectedBoneTransform = SelectedRefBonePose[SelectedBoneIndex];
			FAnimNode_ModifyBone& PreviewModifyBone = DebugSkeletalComponent->PreviewInstance->ModifyBone(BoneName);

			PreviewModifyBone.TranslationMode = EBoneModificationMode::BMM_Replace;
			PreviewModifyBone.RotationMode = EBoneModificationMode::BMM_Replace;
			PreviewModifyBone.ScaleMode = EBoneModificationMode::BMM_Replace;

			PreviewModifyBone.TranslationSpace = EBoneControlSpace::BCS_ParentBoneSpace;
			PreviewModifyBone.RotationSpace = EBoneControlSpace::BCS_ParentBoneSpace;
			PreviewModifyBone.ScaleSpace = EBoneControlSpace::BCS_ParentBoneSpace;

			PreviewModifyBone.Translation = SelectedBoneTransform.GetTranslation();
			PreviewModifyBone.Rotation = SelectedBoneTransform.Rotator();
			PreviewModifyBone.Scale = SelectedBoneTransform.GetScale3D();
		}
	}

	////dirty it
	EdittingSkeletalMesh->PostEditChange();
	EdittingSkeletalMesh->MarkPackageDirty();
}

void UCharacterCreationEditorLibrary::SaveCurrentReferencePose(UDebugSkelMeshComponent* SkeletalComponent)
{
#pragma region CheckAndDeclarartions
	if (!SkeletalComponent)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because UDebugSkelMeshComponent is null."));
		//TODO: Create alert
		return;
	}
	USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(SkeletalComponent->GetSkinnedAsset());
	if (!SkeletalMesh)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because The SkeletalMesh of the SkeletalMeshComponent is null."));
		//TODO: Create alert
		return;
	}
	FSkeletalMeshModel* ImportedModel = SkeletalMesh->GetImportedModel();
	if (!ImportedModel)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because The ImportedModel of the TargetSkeletalMesh is null."));
		//TODO: Create alert
		return;
	}
#pragma endregion

	FScopedTransaction const Transaction(LOCTEXT("SaveCurrentReferencePose", "Save Current Reference Pose"));

	SkeletalMesh->Modify();
#pragma region RefSkeleton
	{

		USkeleton* Skeleton = SkeletalMesh->GetSkeleton();
		FReferenceSkeleton& RefSkeleton = SkeletalMesh->GetRefSkeleton();
		FReferenceSkeletonModifier RefSkeletonModifier(RefSkeleton, Skeleton);

		//TArray<FTransform> PreviewBoneTransforms = DebugSkeletalComponent->GetComponentSpaceTransforms();
		////DebugSkeletalComponent->PreviewInstance->ResetModifiedBone();//So it's no longer modified by Bones;

		//const TArray<FTransform>& MeshRefPose = RefSkeleton.GetRefBonePose();//This include skeleton's virtual bones

		//for (int32 MeshRefBoneIndex = 0; MeshRefBoneIndex < MeshRefPose.Num(); ++MeshRefBoneIndex)
		//{
		//	RefSkeletonModifier.UpdateRefPoseTransform(MeshRefBoneIndex, PreviewBoneTransforms[MeshRefBoneIndex]);
		//}
		//DebugSkeletalComponent->PreviewInstance->ResetModifiedBone();//So it's no longer modified by Bones;
		//TargetSkeletalMesh->WaitForPendingInitOrStreaming();


		const TArray<FTransform>& SpaceBases = SkeletalComponent->GetComponentSpaceTransforms();
		// @todo check to see if skeleton vs preview mesh makes it different for missing bones
		TArray<FTransform>& NewRetargetBasePose = SkeletalMesh->GetRetargetBasePose();
		// if you're using master pose component in preview, this won't work
		check(RefSkeleton.GetNum() == SpaceBases.Num());
		int32 TotalNumBones = SkeletalMesh->GetRefSkeleton().GetNum();
		NewRetargetBasePose.Empty(TotalNumBones);
		NewRetargetBasePose.AddUninitialized(TotalNumBones);

		for (int32 BoneIndex = 0; BoneIndex < TotalNumBones; ++BoneIndex)
		{
			// this is slower, but skeleton can have more bones, so I can't just access
			// Parent index from Skeleton. Going safer route
			FName BoneName = SkeletalComponent->GetBoneName(BoneIndex);
			FName ParentBoneName = SkeletalComponent->GetParentBone(BoneName);
			int32 ParentIndex = RefSkeleton.FindBoneIndex(ParentBoneName);

			if (ParentIndex != INDEX_NONE)
			{
				NewRetargetBasePose[BoneIndex] = SpaceBases[BoneIndex].GetRelativeTransform(SpaceBases[ParentIndex]);
			}
			else
			{
				NewRetargetBasePose[BoneIndex] = SpaceBases[BoneIndex];
			}
			RefSkeletonModifier.UpdateRefPoseTransform(BoneIndex, NewRetargetBasePose[BoneIndex]);
		}

		// Clear PreviewMeshComp bone modified, they're baked now
		SkeletalComponent->PreviewInstance->ResetModifiedBone();
	}
#pragma endregion

	SkeletalMesh->WaitForPendingInitOrStreaming();

#pragma region BakeVertices
	{
		TArray<FFinalSkinVertex> PosedVertexPositions;
		SkeletalComponent->GetCPUSkinnedVertices(PosedVertexPositions, 0);
		TArray<FVector> RawPositions;
		TArray<FSoftSkinVertex> ImportedVertices;
		ImportedModel->LODModels[0].GetVertices(ImportedVertices);


		for (int32 VertexIndex = 0; VertexIndex < PosedVertexPositions.Num(); VertexIndex++)
		{
			ImportedVertices[VertexIndex].Position = PosedVertexPositions[VertexIndex].Position;
		}
		//save LOD import data, and make new guid to force DDC re-creation
		SkeletalMesh->InvalidateDeriveDataCacheGUID();
		SkeletalMesh->Build();
		SkeletalMesh->GetRefBasesInvMatrix().Empty();
		SkeletalMesh->CalculateInvRefMatrices();
	}
#pragma endregion

	//dirty it
	SkeletalMesh->PostEditChange();
	SkeletalMesh->MarkPackageDirty();
}
*/


void UCharacterCreationEditorLibrary::ResetModifiedBones(UDebugSkelMeshComponent* Component)
{
	Component->PreviewInstance->ResetModifiedBone();
}


void UCharacterCreationEditorLibrary::BakeCurrentGeometry(UDebugSkelMeshComponent* SkeletalComponent)
{
#pragma region CheckAndDeclarartions
	if (!SkeletalComponent)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because UDebugSkelMeshComponent is null."));
		//TODO: Create alert
		return;
	}
	USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(SkeletalComponent->GetSkinnedAsset());
	if (!SkeletalMesh)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because The SkeletalMesh of the SkeletalMeshComponent is null."));
		//TODO: Create alert
		return;
	}
	FSkeletalMeshModel* ImportedModel = SkeletalMesh->GetImportedModel();
	if (!ImportedModel)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because The ImportedModel of the TargetSkeletalMesh is null."));
		//TODO: Create alert
		return;
	}

	SkeletalMesh->WaitForPendingInitOrStreaming();

	FSkeletalMeshRenderData* RenderData = SkeletalMesh->GetResourceForRendering();
	if (!RenderData)
	{
		UE_LOG(CharacterCreationEditorLog, Error, TEXT("Failed because The RenderData of the TargetSkeletalMesh is null."));
		//TODO: Create alert
		return;
	}


#pragma endregion

	FScopedTransaction const Transaction(LOCTEXT("SaveCurrentReferencePose", "Save Current Reference Pose"));

	SkeletalMesh->WaitForPendingInitOrStreaming();

#pragma region BakeVertices
	{
		//ImportedModel->GenerateNewGUID();

		for (int32 LODIndex = 0; LODIndex < ImportedModel->LODModels.Num(); ++LODIndex)
		{

			FSkeletalMeshLODRenderData& LODRenderData = RenderData->LODRenderData[LODIndex];



			SkeletalMesh->Modify();
			SkeletalMesh->InvalidateDeriveDataCacheGUID();


			TArray<FFinalSkinVertex> PosedVertexPositions;
			SkeletalComponent->GetCPUSkinnedVertices(PosedVertexPositions, LODIndex);

			FSkeletalMeshLODModel& ImportedMeshLodModel = ImportedModel->LODModels[LODIndex];
			TArray<FSkelMeshSection>& LodSections = ImportedMeshLodModel.Sections;


			for (int32 i = 0; i < PosedVertexPositions.Num(); i++)
			{
				int32 SectionIndex, SectionVertexIndex;

				ImportedMeshLodModel.GetSectionFromVertexIndex(i, SectionIndex, SectionVertexIndex);

				FSoftSkinVertex& DestVertex = ImportedMeshLodModel.Sections[SectionIndex].SoftVertices[SectionVertexIndex];

				DestVertex.Position = PosedVertexPositions[i].Position;
				//uint8 W = DestVertex.TangentZ.W;
				//DestVertex.TangentZ = FVector(NewNormal);
				//DestVertex.TangentZ.W = W;
			}




			//int32 SectionIndex = -1;

			//for (FSkelMeshSection& Section : LodSections)
			//{
			//	SectionIndex++;
			//	const FSkelMeshRenderSection& RenderSection = LODRenderData.RenderSections[SectionIndex];

			//	FSoftSkinVertex* DestVertex = Section.SoftVertices.GetData();
			//	TArray<FSoftSkinVertex>& SoftVerts = Section.SoftVertices;
			//	int32 const BaseIndex = Section.BaseIndex;

			//	
			//	ParallelFor(Section.NumTriangles, [&](int32 TriangleIndex)
			//		{
			//			for (uint32 PointIndex = 0; PointIndex < 3; PointIndex++)
			//			{
			//				int32 const Index = PointIndex + (TriangleIndex * 3);
			//				//int32 const IndexBufferValue = ImportedMeshLodModel.IndexBuffer[Section.BaseIndex + Index];
			//				int32 const IndexBufferValue = BaseIndex + Index;
			//				if (SoftVerts.IsValidIndex(Index) && PosedVertexPositions.IsValidIndex(IndexBufferValue))
			//				{
			//					SoftVerts[Index].Position = PosedVertexPositions[IndexBufferValue].Position;
			//				}
			//				else {
			//					UE_LOG(LogTemp, Warning, TEXT("Invalid Indeces: Index: %s IndexBufferValue: %s"), *FString::FromInt(Index), *FString::FromInt(IndexBufferValue));
			//				}
			//			}
			//		});



			//}

			SkeletalMesh->MarkPackageDirty();
			SkeletalMesh->GetOutermost()->SetDirtyFlag(true);
		}
	}


#pragma endregion

	//dirty it
	SkeletalMesh->InvalidateDeriveDataCacheGUID();
	SkeletalMesh->PostEditChange();
	SkeletalMesh->MarkPackageDirty();
	SkeletalMesh->PostEditChange();
	SkeletalMesh->PostLoad();
}

void UCharacterCreationEditorLibrary::LogSekeleton(const USkeleton* SelectedSkeleton)
{

}

void UCharacterCreationEditorLibrary::LogSekeletalMesh(const USkeletalMesh* SkeletalMesh)
{
	const FReferenceSkeleton& RefSkeleton = SkeletalMesh->GetRefSkeleton();
	LogReferenceSkeleton(RefSkeleton);

	const TArray<FSkinWeightProfileInfo>& SkinWeightProfiles = SkeletalMesh->GetSkinWeightProfiles();



}

void UCharacterCreationEditorLibrary::LogReferenceSkeleton(const FReferenceSkeleton& EdittingRefSkeleton)
{
	const TArray<FMeshBoneInfo>& BoneInfoArray = EdittingRefSkeleton.GetRefBoneInfo();

	for (const FMeshBoneInfo& BoneInfo : BoneInfoArray)
	{
		UE_LOG(CharacterCreationEditorLog, Log, TEXT("Name: %s ExportName: %s ParentIndex: %i"), *BoneInfo.Name.ToString(), *BoneInfo.ExportName, BoneInfo.ParentIndex);
	}
}


bool UCharacterCreationEditorLibrary::MergeBonesToBoneTreeDeletingIntermediate(USkeleton* const Skeleton, USkeletalMesh* const SkeletalMesh, const TArray<int32>& RequiredRefBones)
{
#pragma region Checks
	if (!Skeleton || !SkeletalMesh)
	{
		return false;
	}
	if (Skeleton == SkeletalMesh->GetSkeleton())
	{
		return true;
	}
#pragma endregion Checks

	FlushRenderingCommands();

	const FReferenceSkeleton& SkeletalRefSkel = Skeleton->GetReferenceSkeleton();
	const TArray<FTransform>& SkeletalRefSkelPose = SkeletalRefSkel.GetRefBonePose();
	const TArray<FMeshBoneInfo>& RefBoneInfo = SkeletalRefSkel.GetRefBoneInfo();

	FReferenceSkeleton& CurrentMeshRefSkeleton = SkeletalMesh->GetRefSkeleton();
	CurrentMeshRefSkeleton.Empty();

	FReferenceSkeletonModifier RefSkelModifier(CurrentMeshRefSkeleton, nullptr);


	for (int32 i = 0; i < RefBoneInfo.Num(); i++)
	{
		const FMeshBoneInfo& NewMeshBoneInfo = RefBoneInfo[i];
		RefSkelModifier.Add(RefBoneInfo[i], SkeletalRefSkelPose[i]);
	}


	SkeletalMesh->CalculateInvRefMatrices();
	SkeletalMesh->Build();
	SkeletalMesh->SetSkeleton(Skeleton);
	SkeletalMesh->MarkPackageDirty();

	return true;

	//for (int32 Index = 0; Index < SkeletalRefSkelPose.Num(); Index++)
	//{
	//	const int32& MeshBoneIndex = FilteredRequiredBones[Index];
	//	const int32& SkeletonBoneIndex = ReferenceSkeleton.FindRawBoneIndex(InSkeletalMesh->GetRefSkeleton().GetBoneName(MeshBoneIndex));

	//	// Bone doesn't already exist. Add it.
	//	if (SkeletonBoneIndex == INDEX_NONE)
	//	{
	//		FMeshBoneInfo NewMeshBoneInfo = InSkeletalMesh->GetRefSkeleton().GetRefBoneInfo()[MeshBoneIndex];
	//		// Fix up ParentIndex for our new Skeleton.
	//		if (ReferenceSkeleton.GetRawBoneNum() == 0)
	//		{
	//			NewMeshBoneInfo.ParentIndex = INDEX_NONE; // root
	//		}
	//		else
	//		{
	//			NewMeshBoneInfo.ParentIndex = ReferenceSkeleton.FindRawBoneIndex(InSkeletalMesh->GetRefSkeleton().GetBoneName(InSkeletalMesh->GetRefSkeleton().GetParentIndex(MeshBoneIndex)));
	//		}

	//		RefSkelModifier.Add(NewMeshBoneInfo, InSkeletalMesh->GetRefSkeleton().GetRefBonePose()[MeshBoneIndex]);
	//		BoneTree.AddZeroed(1);
	//		bShouldHandleHierarchyChange = true;
	//	}
	//}





	//from Skeleton
	//bool USkeleton::MergeBonesToBoneTree(const USkeletalMesh * InSkeletalMesh, const TArray<int32> &RequiredRefBones)
	//{
//		// see if it needs all animation data to remap - only happens when bone structure CHANGED - added
//		bool bSuccess = false;
//		bool bShouldHandleHierarchyChange = false;
//		// clear cache data since it won't work anymore once this is done
//		ClearCacheData();
//
//		// if it's first time
//		if (BoneTree.Num() == 0)
//		{
//			bSuccess = CreateReferenceSkeletonFromMesh(InSkeletalMesh, RequiredRefBones);
//			bShouldHandleHierarchyChange = true;
//		}
//		else
//		{
//			// can we play? - hierarchy matches
//			if (IsCompatibleMesh(InSkeletalMesh))
//			{
//				// Exclude bones who do not have a parent.
//				TArray<int32> FilteredRequiredBones;
//				FAnimationRuntime::ExcludeBonesWithNoParents(RequiredRefBones, InSkeletalMesh->GetRefSkeleton(), FilteredRequiredBones);
//
//				FReferenceSkeletonModifier RefSkelModifier(ReferenceSkeleton, this);
//
//				for (int32 Index = 0; Index < FilteredRequiredBones.Num(); Index++)
//				{
//					const int32& MeshBoneIndex = FilteredRequiredBones[Index];
//					const int32& SkeletonBoneIndex = ReferenceSkeleton.FindRawBoneIndex(InSkeletalMesh->GetRefSkeleton().GetBoneName(MeshBoneIndex));
//
//					// Bone doesn't already exist. Add it.
//					if (SkeletonBoneIndex == INDEX_NONE)
//					{
//						FMeshBoneInfo NewMeshBoneInfo = InSkeletalMesh->GetRefSkeleton().GetRefBoneInfo()[MeshBoneIndex];
//						// Fix up ParentIndex for our new Skeleton.
//						if (ReferenceSkeleton.GetRawBoneNum() == 0)
//						{
//							NewMeshBoneInfo.ParentIndex = INDEX_NONE; // root
//						}
//						else
//						{
//							NewMeshBoneInfo.ParentIndex = ReferenceSkeleton.FindRawBoneIndex(InSkeletalMesh->GetRefSkeleton().GetBoneName(InSkeletalMesh->GetRefSkeleton().GetParentIndex(MeshBoneIndex)));
//						}
//
//						RefSkelModifier.Add(NewMeshBoneInfo, InSkeletalMesh->GetRefSkeleton().GetRefBonePose()[MeshBoneIndex]);
//						BoneTree.AddZeroed(1);
//						bShouldHandleHierarchyChange = true;
//					}
//				}
//
//				bSuccess = true;
//			}
//		}
//
//		// if succeed
//		if (bShouldHandleHierarchyChange)
//		{
//#if WITH_EDITOR
//			HandleSkeletonHierarchyChange();
//#endif
//		}
//
//		return bSuccess;
//	}

	//End From Skeleton


	//Original Code:
	//if (RequiredBones.Num() > 0)
	//{
	//	// Do automatic asset generation.
	//	bool const bSuccess = SelectedSkeleton->MergeBonesToBoneTree(SkeletalMesh, RequiredBones);
	//	if (bSuccess)
	//	{
	//		if (SkelMesh->GetSkeleton() != SelectedSkeleton)
	//		{
	//			SkelMesh->SetSkeleton(SelectedSkeleton);
	//			SkelMesh->MarkPackageDirty();
	//			return true;
	//		}
	//	}
	//}
}


#undef LOCTEXT_NAMESPACE
