// Created by Jorge Puerto. All Rights Reserved.

#include "MeshEdittingLibrary.h"
#include "MeshTypes.h"
#include "StaticMeshAttributes.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/MorphTarget.h"
#include "Rendering/SkeletalMeshModel.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Animation/Skeleton.h"
#include "Editor.h"
#include "GenericQuadTree.h"
#include "Async/ParallelFor.h"
#include "Modules/ModuleManager.h"
#include "Misc/PackageName.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "Dialogs/DlgPickAssetPath.h"
#include "AssetRegistry/AssetRegistryModule.h"
//#include "MeshMorpherRuntimeLibrary.h"
#include "BAMorphTarget.h"
#include "FileHelpers.h"
#include "Misc/MessageDialog.h"
#include "MeshDescriptionToDynamicMesh.h"
#include "DynamicMeshToMeshDescription.h"
#include "DynamicMesh/MeshNormals.h"
#include "MeshUtilities.h"
#include "RawMesh.h"
#include "Modules/ModuleManager.h"
#include "Misc/FeedbackContext.h"
#include "Misc/App.h"
#include "DynamicMesh/Operations/MergeCoincidentMeshEdges.h"
#ifndef ENGINE_MINOR_VERSION
#include "Runtime/Launch/Resources/Version.h"
#endif

#include "StaticMeshOperations.h"

#include "Engine/SkeletalMeshEditorData.h"

#include "Misc/FileHelper.h"
#include "CharacterCreationSettings.h"
#include "MeshAttributes.h"
#include "Engine/SkinnedAssetCommon.h"


#define LOCTEXT_NAMESPACE "MeshMorpherOperations"
void UMeshEdittingLibrary::NotifyMessage(const FString& Message)
{
	auto result = FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Message));
}

bool UMeshEdittingLibrary::CopySkeletalMeshMaterialsToStaticMesh(USkeletalMesh* SkeletalMesh, UStaticMesh* StaticMesh)
{
	if (SkeletalMesh && StaticMesh)
	{

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27) || ENGINE_MAJOR_VERSION == 5
		TArray<FStaticMaterial>& StaticMaterials = StaticMesh->GetStaticMaterials();
#else
		TArray<FStaticMaterial>& StaticMaterials = StaticMesh->StaticMaterials;
#endif

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27) || ENGINE_MAJOR_VERSION == 5
		TArray<FSkeletalMaterial>& SkeletalMaterials = SkeletalMesh->GetMaterials();
#else
		TArray<FSkeletalMaterial>& SkeletalMaterials = SkeletalMesh->Materials;
#endif

		StaticMaterials.Empty();

		for (int32 MaterialIndex = 0; MaterialIndex < SkeletalMaterials.Num(); ++MaterialIndex)
		{
			FStaticMaterial StaticMaterial;
			const FName Slot = *FString("MAT" + FString::FromInt(MaterialIndex));
			StaticMaterial.ImportedMaterialSlotName = Slot;
			StaticMaterial.MaterialInterface = SkeletalMaterials[MaterialIndex].MaterialInterface;
			StaticMaterial.MaterialSlotName = Slot;
			StaticMaterial.UVChannelData = SkeletalMaterials[MaterialIndex].UVChannelData;
			StaticMaterials.Add(StaticMaterial);
		}
		return true;
	}
	return false;
}

bool UMeshEdittingLibrary::CopySkeletalMeshMaterials(USkeletalMesh* SkeletalMesh, TArray<FStaticMaterial>& StaticMaterials)
{
	if (SkeletalMesh)
	{
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27) || ENGINE_MAJOR_VERSION == 5
		TArray<FSkeletalMaterial>& SkeletalMaterials = SkeletalMesh->GetMaterials();
#else
		TArray<FSkeletalMaterial>& SkeletalMaterials = SkeletalMesh->Materials;
#endif

		for (int32 MaterialIndex = 0; MaterialIndex < SkeletalMaterials.Num(); ++MaterialIndex)
		{
			FStaticMaterial StaticMaterial;
			const FName Slot = *FString("MAT" + FString::FromInt(MaterialIndex));
			StaticMaterial.ImportedMaterialSlotName = Slot;
			StaticMaterial.MaterialInterface = SkeletalMaterials[MaterialIndex].MaterialInterface;
			StaticMaterial.MaterialSlotName = Slot;
			StaticMaterial.UVChannelData = SkeletalMaterials[MaterialIndex].UVChannelData;
			StaticMaterials.Add(StaticMaterial);
		}
		return true;
	}
	return false;
}

bool UMeshEdittingLibrary::CopySkeletalMeshMaterials(USkeletalMesh* SkeletalMesh, TMap<int32, FName>& StaticMaterials)
{
	if (SkeletalMesh)
	{
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27) || ENGINE_MAJOR_VERSION == 5
		TArray<FSkeletalMaterial>& SkeletalMaterials = SkeletalMesh->GetMaterials();
#else
		TArray<FSkeletalMaterial>& SkeletalMaterials = SkeletalMesh->Materials;
#endif
		StaticMaterials.Empty();
		for (int32 MaterialIndex = 0; MaterialIndex < SkeletalMaterials.Num(); ++MaterialIndex)
		{
			FStaticMaterial StaticMaterial;
			const FName Slot = *FString("MAT" + FString::FromInt(MaterialIndex));
			StaticMaterials.Add(MaterialIndex, Slot);
		}
		return true;
	}
	return false;
}

bool UMeshEdittingLibrary::CopySkeletalMeshMaterials(USkeletalMesh* SkeletalMesh, TMap<FName, int32>& StaticMaterials)
{
	if (SkeletalMesh)
	{
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27) || ENGINE_MAJOR_VERSION == 5
		TArray<FSkeletalMaterial>& SkeletalMaterials = SkeletalMesh->GetMaterials();
#else
		TArray<FSkeletalMaterial>& SkeletalMaterials = SkeletalMesh->Materials;
#endif
		StaticMaterials.Empty();
		for (int32 MaterialIndex = 0; MaterialIndex < SkeletalMaterials.Num(); ++MaterialIndex)
		{
			FStaticMaterial StaticMaterial;
			const FName Slot = *FString("MAT" + FString::FromInt(MaterialIndex));
			StaticMaterials.Add(Slot, MaterialIndex);
		}
		return true;
	}
	return false;
}


bool UMeshEdittingLibrary::SetStaticMesh(UStaticMesh* StaticMesh, const FMeshDescription& MeshDescription)
{
	if (StaticMesh)
	{
		TArray <const FMeshDescription*> MD;
		MD.Add(&MeshDescription);
		StaticMesh->BuildFromMeshDescriptions(MD);
		return true;
	}
	return false;
}

bool UMeshEdittingLibrary::SkeletalMeshToDynamicMesh(USkeletalMesh* SkeletalMesh, UE::Geometry::FDynamicMesh3& IdenticalDynamicMesh, UE::Geometry::FDynamicMesh3* WeldedDynamicMesh, const TArray<FFinalSkinVertex>& FinalVertices, int32 LOD, bool bUseRenderData)
{
	return SkeletalMeshToDynamicMesh_RenderData(SkeletalMesh, IdenticalDynamicMesh, WeldedDynamicMesh, FinalVertices, LOD);
}


bool UMeshEdittingLibrary::SkeletalMeshToDynamicMesh_RenderData(USkeletalMesh* SkeletalMesh, UE::Geometry::FDynamicMesh3& IdenticalDynamicMesh, UE::Geometry::FDynamicMesh3* WeldedDynamicMesh, const TArray<FFinalSkinVertex>& FinalVertices, int32 LOD)
{
//	if (SkeletalMesh)
//	{
//		SkeletalMesh->WaitForPendingInitOrStreaming();
//		const FSkeletalMeshRenderData* Resource = SkeletalMesh->GetResourceForRendering();
//		if (Resource)
//		{
//			if (Resource->LODRenderData.IsValidIndex(LOD))
//			{
//				FMeshDescription IdenticalMeshDescription;
//				IdenticalMeshDescription.Empty();
//				FStaticMeshAttributes StaticMeshAttributesIdentical(IdenticalMeshDescription);
//				StaticMeshAttributesIdentical.Register();
//				IdenticalDynamicMesh.Clear();
//
//				const FSkeletalMeshLODInfo& SrcLODInfo = *(SkeletalMesh->GetLODInfo(LOD));
//				const FSkeletalMeshLODRenderData& LODModel = Resource->LODRenderData[LOD];
//
//				const int32 Numverts = LODModel.StaticVertexBuffers.PositionVertexBuffer.GetNumVertices();
//				bool bUseFinalVertices = Numverts == FinalVertices.Num();
//
//				const int32 NumSections = LODModel.RenderSections.Num();
//				int32 TotalTriangles = 0;
//				for (int32 SectionIndex = 0; SectionIndex < NumSections; SectionIndex++)
//				{
//					TotalTriangles += LODModel.RenderSections[SectionIndex].NumTriangles;
//				}
//				const int32 TotalCorners = TotalTriangles * 3;
//
//				const int32 UVTexNum = bUseFinalVertices ? 1 : LODModel.GetNumTexCoords();
//
//				TPolygonGroupAttributesRef<FName> PolygonGroupImportedMaterialSlotNames = StaticMeshAttributesIdentical.GetPolygonGroupMaterialSlotNames();
//				auto VertexPositions = StaticMeshAttributesIdentical.GetVertexPositions();
//				auto VertexInstanceTangents = StaticMeshAttributesIdentical.GetVertexInstanceTangents();
//				auto VertexInstanceBinormalSigns = StaticMeshAttributesIdentical.GetVertexInstanceBinormalSigns();
//				auto VertexInstanceNormals = StaticMeshAttributesIdentical.GetVertexInstanceNormals();
//				auto VertexInstanceColors = StaticMeshAttributesIdentical.GetVertexInstanceColors();
//				auto VertexInstanceUVs = StaticMeshAttributesIdentical.GetVertexInstanceUVs();
//
//				IdenticalMeshDescription.ReserveNewVertices(Numverts);
//				IdenticalMeshDescription.ReserveNewPolygons(TotalTriangles);
//				IdenticalMeshDescription.ReserveNewVertexInstances(TotalCorners);
//				IdenticalMeshDescription.ReserveNewEdges(TotalCorners);
//
//
//				const bool bCreatedWeldedDynamicMesh = WeldedDynamicMesh != NULL;
//
//
//				VertexInstanceUVs.SetNumChannels(UVTexNum);
//
//#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27) || ENGINE_MAJOR_VERSION == 5
//				TArray<FSkeletalMaterial>& SkeletalMaterials = SkeletalMesh->GetMaterials();
//#else
//				TArray<FSkeletalMaterial>& SkeletalMaterials = SkeletalMesh->Materials;
//#endif
//
//				for (int32 SectionIndex = 0; SectionIndex < NumSections; SectionIndex++)
//				{
//					const FSkelMeshRenderSection& SkelMeshSection = LODModel.RenderSections[SectionIndex];
//					int32 SectionMaterialIndex = SkelMeshSection.MaterialIndex;
//
//					// use the remapping of material indices if there is a valid value
//					if (SrcLODInfo.LODMaterialMap.IsValidIndex(SectionIndex) && SrcLODInfo.LODMaterialMap[SectionIndex] != INDEX_NONE)
//					{
//						SectionMaterialIndex = FMath::Clamp<int32>(SrcLODInfo.LODMaterialMap[SectionIndex], 0, SkeletalMaterials.Num());
//					}
//
//
//					FName ImportedMaterialSlotName = *FString("MAT" + FString::FromInt(SectionMaterialIndex));
//					const FPolygonGroupID SectionPolygonGroupID(SectionIndex);
//					if (!IdenticalMeshDescription.IsPolygonGroupValid(SectionPolygonGroupID))
//					{
//						IdenticalMeshDescription.CreatePolygonGroupWithID(SectionPolygonGroupID);
//						PolygonGroupImportedMaterialSlotNames[SectionPolygonGroupID] = ImportedMaterialSlotName;
//					}
//					for (uint32 SectionTriangleIndex = 0; SectionTriangleIndex < SkelMeshSection.NumTriangles; ++SectionTriangleIndex)
//					{
//						TArray<FVertexInstanceID> VertexInstanceIDs;
//						VertexInstanceIDs.SetNum(3);
//
//						for (int32 CornerIndex = 0; CornerIndex < 3; ++CornerIndex)
//						{
//							const int32 Index = SkelMeshSection.BaseIndex + ((SectionTriangleIndex * 3) + CornerIndex);
//							const int32 WedgeIndex = LODModel.MultiSizeIndexContainer.GetIndexBuffer()->Get(Index);
//
//							const FVertexID VertexID(WedgeIndex);
//
//							if (!IdenticalMeshDescription.IsVertexValid(VertexID))
//							{
//								const auto Position = bUseFinalVertices ? FinalVertices[WedgeIndex].Position : LODModel.StaticVertexBuffers.PositionVertexBuffer.VertexPosition(WedgeIndex);
//								IdenticalMeshDescription.CreateVertexWithID(VertexID);
//								VertexPositions[VertexID] = Position;
//							}
//
//							FVertexInstanceID VertexInstanceID = IdenticalMeshDescription.CreateVertexInstance(VertexID);
//							VertexInstanceIDs[CornerIndex] = VertexInstanceID;
//
//							for (int32 UVLayerIndex = 0; UVLayerIndex < UVTexNum; UVLayerIndex++)
//							{
//								const auto UV = bUseFinalVertices ? FVector2D(FinalVertices[WedgeIndex].U, FinalVertices[WedgeIndex].V) : LODModel.StaticVertexBuffers.StaticMeshVertexBuffer.GetVertexUV(WedgeIndex, UVLayerIndex);
//								VertexInstanceUVs.Set(VertexInstanceID, UVLayerIndex, UV);
//							}
//
//							const auto TangentX = bUseFinalVertices ? FinalVertices[WedgeIndex].TangentX.ToFVector() : LODModel.StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentX(WedgeIndex);
//							const auto TangentY = bUseFinalVertices ? FinalVertices[WedgeIndex].GetTangentY() : LODModel.StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentY(WedgeIndex);
//							const auto TangentZ = bUseFinalVertices ? FinalVertices[WedgeIndex].TangentZ.ToFVector4() : LODModel.StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(WedgeIndex);
//
//							VertexInstanceTangents[VertexInstanceID] = TangentX;
//							VertexInstanceNormals[VertexInstanceID] = TangentZ;
//							VertexInstanceBinormalSigns[VertexInstanceID] = GetBasisDeterminantSign(TangentX.GetSafeNormal(), TangentY.GetSafeNormal(), TangentZ.GetSafeNormal());
//
//							if ((uint32)WedgeIndex < LODModel.StaticVertexBuffers.ColorVertexBuffer.GetNumVertices())
//							{
//								VertexInstanceColors[VertexInstanceID] = FLinearColor(LODModel.StaticVertexBuffers.ColorVertexBuffer.VertexColor(WedgeIndex));
//							}
//							else {
//								VertexInstanceColors[VertexInstanceID] = FLinearColor::White;
//							}
//						}
//						const FPolygonID NewPolygonID = IdenticalMeshDescription.CreatePolygon(SectionPolygonGroupID, VertexInstanceIDs);
//					}
//				}
//
//				FMeshDescriptionToDynamicMesh Converter;
//				Converter.Convert(&IdenticalMeshDescription, IdenticalDynamicMesh);
//
//				if (bCreatedWeldedDynamicMesh)
//				{
//					WeldedDynamicMesh->Copy(IdenticalDynamicMesh);
//					FMergeCoincidentMeshEdges Merger(WeldedDynamicMesh);
//					Merger.MergeVertexTolerance = FMathf::ZeroTolerance;
//					Merger.MergeSearchTolerance = 2 * Merger.MergeVertexTolerance;
//					Merger.OnlyUniquePairs = false;
//					if (Merger.Apply())
//					{
//						WeldedDynamicMesh->CompactInPlace();
//
//						FDynamicMeshNormalOverlay* Normals = WeldedDynamicMesh->HasAttributes() ? WeldedDynamicMesh->Attributes()->PrimaryNormals() : nullptr;
//
//						if (Normals)
//						{
//							FMeshNormals::InitializeOverlayToPerVertexNormals(Normals);
//						}
//					}
//				}
//				return true;
//			}
//		}
//	}
	return false;
}


void CreateEmptyLODModel(FMorphTargetLODModel& LODModel)
{
	//FMorphTargetDelta EmptyDelta;
	//EmptyDelta.PositionDelta = FVector::ZeroVector;
	//EmptyDelta.TangentZDelta = FVector::ZeroVector;
	//EmptyDelta.SourceIdx = 0;
	//TArray<FMorphTargetDelta> Deltas;
	//Deltas.Add(EmptyDelta);
	//LODModel.Vertices = Deltas;
	//LODModel.SectionIndices.Add(0);
	//LODModel.NumBaseMeshVerts = 1;
	//LODModel.bGeneratedByEngine = false;
}

UMorphTarget* UMeshEdittingLibrary::FindMorphTarget(USkeletalMesh* Mesh, FString MorphTargetName)
{
//	if (Mesh)
//	{
//#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27) || ENGINE_MAJOR_VERSION == 5
//		TArray<UMorphTarget*>& LocalMorphTargets = Mesh->GetMorphTargets();
//#else
//		TArray<UMorphTarget*>& LocalMorphTargets = Mesh->MorphTargets;
//#endif
//
//		for (UMorphTarget* MorphTargetObj : LocalMorphTargets)
//		{
//			if (MorphTargetObj && !MorphTargetObj->IsPendingKillOrUnreachable())
//			{
//				if (MorphTargetObj->GetName().Equals(MorphTargetName))
//				{
//					return MorphTargetObj;
//				}
//			}
//		}
//	}
	return nullptr;
}


void UMeshEdittingLibrary::CreateMorphTargetObj(USkeletalMesh* Mesh, FString MorphTargetName, bool bInvalidateRenderData)
{
	//checkf(Mesh, TEXT("Invalid skeletal mesh."));

	//UMorphTarget* MorphTargetObj = FindMorphTarget(Mesh, MorphTargetName);
	//bool bCreated = false;
	//if (!MorphTargetObj)
	//{
	//	Mesh->Modify();
	//	Mesh->InvalidateDeriveDataCacheGUID();
	//	MorphTargetObj = NewObject<UMorphTarget>(Mesh, FName(*FString::Printf(TEXT("%s"), *MorphTargetName)));
	//}

	//MorphTargetObj->MorphLODModels.Empty();

	//auto& MorphData = MorphTargetObj->MorphLODModels.AddDefaulted_GetRef();
	//FMorphTargetDelta EmptyDelta;
	//EmptyDelta.PositionDelta = FVector::ZeroVector;
	//EmptyDelta.TangentZDelta = FVector::ZeroVector;
	//EmptyDelta.SourceIdx = Mesh->GetImportedModel()->LODModels[0].Sections[0].BaseVertexIndex;
	//TArray<FMorphTargetDelta> Deltas;
	//Deltas.Add(EmptyDelta);
	//MorphData.Vertices = Deltas;
	//MorphData.SectionIndices.Add(0);
	//MorphData.NumBaseMeshVerts = Mesh->GetImportedModel()->LODModels[0].NumVertices;
	//MorphData.bGeneratedByEngine = false;
	//MorphTargetObj->BaseSkelMesh = Mesh;

	//bool bRegistered = Mesh->RegisterMorphTarget(MorphTargetObj, bInvalidateRenderData);
	//MorphTargetObj->MarkPackageDirty();
}


void UMeshEdittingLibrary::CreateMorphTarget(USkeletalMesh* Mesh, FString MorphTargetName)
{
	/*checkf(Mesh, TEXT("Invalid skeletal mesh."));

	if (Mesh)
	{
		FSkeletalMeshModel* ResourceImported = Mesh->GetImportedModel();
		if (ResourceImported)
		{

			if (ResourceImported->LODModels.IsValidIndex(0))
			{
				FSkeletalMeshLODModel& LODModel = ResourceImported->LODModels[0];
				FSkeletalMeshImportData RawMesh;


				Mesh->LoadLODImportedData(0, RawMesh);

				if (RawMesh.Points.Num())
				{

					Mesh->Modify();
					Mesh->InvalidateDeriveDataCacheGUID();

					int32 ImportMorphIndex = RawMesh.MorphTargetNames.IndexOfByKey(MorphTargetName);
					if (ImportMorphIndex == INDEX_NONE)
					{
						ImportMorphIndex = RawMesh.MorphTargetNames.Add(MorphTargetName);
						RawMesh.MorphTargets.AddDefaulted();
						RawMesh.MorphTargetModifiedPoints.AddDefaulted();
					}
					else {
						RawMesh.MorphTargets[ImportMorphIndex].Points.Empty();
						RawMesh.MorphTargetModifiedPoints[ImportMorphIndex].Empty();
					}

					if (LODModel.MeshToImportVertexMap.IsValidIndex(0))
					{
						const int32 RawIndex = LODModel.MeshToImportVertexMap[0];
						if (RawMesh.Points.IsValidIndex(RawIndex))
						{
							RawMesh.MorphTargets[ImportMorphIndex].Points.Add(RawMesh.Points[RawIndex] + FVector::ZeroVector);
							RawMesh.MorphTargetModifiedPoints[ImportMorphIndex].Add(RawIndex);
						}
					}
					Mesh->SaveLODImportedData(0, RawMesh);

					Mesh->MarkPackageDirty();
				}
				else {
					CreateMorphTargetObj(Mesh, MorphTargetName);
				}
			}
		}
	}*/
}

void UMeshEdittingLibrary::ApplyDeltasToDynamicMesh(const TArray<FMorphTargetDelta>& Deltas, UE::Geometry::FDynamicMesh3& DynamicMesh)
{
	/*if (Deltas.Num() > 0)
	{
		ParallelFor(Deltas.Num(), [&](int32 Index)
			{
				const FMorphTargetDelta& Vert = Deltas[Index];
				if (DynamicMesh.IsVertex(Vert.SourceIdx))
				{
					const FVector3d Position = DynamicMesh.GetVertex(Vert.SourceIdx) + FVector3d(Vert.PositionDelta);
					DynamicMesh.SetVertex(Vert.SourceIdx, Position);
				}
			});

		ParallelFor(DynamicMesh.VertexCount(), [&](int32 Index)
			{
				if (DynamicMesh.IsVertex(Index))
				{
					DynamicMesh.SetVertexNormal(Index, FVector3f(FMeshNormals::ComputeVertexNormal(DynamicMesh, Index)));
				}
			});
	}*/
}

void UMeshEdittingLibrary::ApplyDeltasToDynamicMesh(UE::Geometry::FDynamicMesh3& SourceDynamicMesh, const TArray<FMorphTargetDelta>& Deltas, UE::Geometry::FDynamicMesh3& DynamicMesh)
{
	/*FFormatNamedArguments Args;
	Args.Add(TEXT("Deltas"), FText::FromString(FString::FromInt(Deltas.Num())));
	const FText StatusUpdate = FText::Format(LOCTEXT("ApplyDeltasToMorphTarget", "({Deltas}) Applying deltas..."), Args);
	GWarn->BeginSlowTask(StatusUpdate, true);
	TArray<FMorphTargetDelta> NewDeltas;
	GWarn->StatusForceUpdate(1, 3, FText::FromString("Convert Deltas to Welded Mesh ..."));
	ApplySourceDeltasToDynamicMesh(SourceDynamicMesh, DynamicMesh, Deltas, NewDeltas);
	GWarn->StatusForceUpdate(2, 3, FText::FromString("Apply Deltas to Welded Mesh ..."));
	ApplyDeltasToDynamicMesh(NewDeltas, DynamicMesh);
	GWarn->StatusForceUpdate(3, 3, FText::FromString("Finished!"));
	GWarn->EndSlowTask();*/
}

void UMeshEdittingLibrary::GetMorphDeltas(const UE::Geometry::FDynamicMesh3& Original, const UE::Geometry::FDynamicMesh3& Changed, TArray<FMorphTargetDelta>& Deltas)
{
	/*Deltas.Empty();

	FCriticalSection Lock;

	ParallelFor(Changed.VertexCount(), [&](int32 Index)
		{
			if (Original.IsVertex(Index))
			{
				const FVector ChangedLocation = FVector(Changed.GetVertex(Index));
				const FVector OriginalLocation = FVector(Original.GetVertex(Index));

				if (!ChangedLocation.Equals(OriginalLocation))
				{
					const FVector NewPosition = ChangedLocation - OriginalLocation;
					if (NewPosition.SizeSquared() > FMath::Square(THRESH_POINTS_ARE_NEAR))
					{
						Lock.Lock();
						const FVector ChangedNormal = FVector(FMeshNormals::ComputeVertexNormal(Changed, Index));
						const FVector OriginalNormal = FVector(FMeshNormals::ComputeVertexNormal(Original, Index));
						FMorphTargetDelta& NewMorphDelta = Deltas.AddZeroed_GetRef();
						NewMorphDelta.PositionDelta = NewPosition;
						NewMorphDelta.TangentZDelta = (ChangedNormal - OriginalNormal);
						NewMorphDelta.SourceIdx = Index;
						Lock.Unlock();
					}
				}
			}
		});*/
}


void UMeshEdittingLibrary::ApplyChangesToMorphTarget(USkeletalMesh* Mesh, const UE::Geometry::FDynamicMesh3& DynamicMesh, FString MorphTargetName, const UE::Geometry::FDynamicMesh3& Original, const UE::Geometry::FDynamicMesh3& Changed)
{
	/*checkf(Mesh, TEXT("Invalid skeletal mesh."));
	checkf(Mesh->GetImportedModel()->LODModels.Num() > 0, TEXT("Invalid LOD for skeletal mesh."));

	FSkeletalMeshModel* SkeletalMeshResource = Mesh->GetImportedModel();
	if (!SkeletalMeshResource || !SkeletalMeshResource->LODModels.IsValidIndex(0))
	{
		return;
	}
	FSkeletalMeshLODModel& BaseLODModel = SkeletalMeshResource->LODModels[0];

	FFormatNamedArguments Args;
	Args.Add(TEXT("MorphTarget"), FText::FromString(MorphTargetName));
	const FText StatusUpdate = FText::Format(LOCTEXT("ApplyChangesToMorphTarget", "({MorphTarget}) Applying changes..."), Args);
	GWarn->BeginSlowTask(StatusUpdate, true);

	TArray<FMorphTargetDelta> Deltas;
	GWarn->StatusUpdate(1, 3, FText::FromString("Get Deltas..."));
	GetMorphDeltas(Original, Changed, Deltas);

	if (Deltas.Num() > 0)
	{
		TArray<FMorphTargetDelta> NewDeltas;
		GWarn->StatusUpdate(2, 3, FText::FromString("Apply Deltas to Skeletal Mesh..."));
		ApplySourceDeltasToDynamicMesh(Original, DynamicMesh, Deltas, NewDeltas);
		if (NewDeltas.Num() > 0)
		{

			GWarn->StatusUpdate(3, 3, FText::FromString("Populate Morph target with Deltas.."));
			ApplyMorphTargetToImportData(Mesh, MorphTargetName, NewDeltas);
		}
	}
	GWarn->EndSlowTask();*/
}

void UMeshEdittingLibrary::RenameMorphTargetInImportData(USkeletalMesh* Mesh, FString NewName, FString OriginalName, bool bInvalidateRenderData)
{
	/*if (Mesh)
	{
		FSkeletalMeshModel* ResourceImported = Mesh->GetImportedModel();
		if (ResourceImported)
		{

			Mesh->Modify();
			Mesh->InvalidateDeriveDataCacheGUID();

			for (int32 LODIdx = 0; LODIdx < ResourceImported->LODModels.Num(); ++LODIdx)
			{
				FSkeletalMeshLODModel& LODModel = ResourceImported->LODModels[LODIdx];
				FSkeletalMeshImportData RawMesh;

				Mesh->LoadLODImportedData(LODIdx, RawMesh);

				int32 ImportMorphIndex = RawMesh.MorphTargetNames.IndexOfByKey(OriginalName);
				if (ImportMorphIndex != INDEX_NONE)
				{
					RawMesh.MorphTargetNames[ImportMorphIndex] = NewName;
					Mesh->SaveLODImportedData(LODIdx, RawMesh);
					Mesh->MarkPackageDirty();
				}
			}
		}

		UMorphTarget* MorphTargetObj = FindMorphTarget(Mesh, OriginalName);
		if (MorphTargetObj)
		{
			MorphTargetObj->Rename(*NewName);
			if (bInvalidateRenderData)
			{
				Mesh->InitMorphTargetsAndRebuildRenderData();
			}
			MorphTargetObj->MarkPackageDirty();
		}
	}*/
}

void UMeshEdittingLibrary::RemoveMorphTargetsFromImportData(USkeletalMesh* Mesh, const TArray<FString>& MorphTargets, bool bInvalidateRenderData)
{
//	if (Mesh && MorphTargets.Num() > 0)
//	{
//		Mesh->Modify();
//		Mesh->InvalidateDeriveDataCacheGUID();
//
//		FSkeletalMeshModel* ResourceImported = Mesh->GetImportedModel();
//
//
//		if (ResourceImported)
//		{
//			for (int32 LODIdx = 0; LODIdx < ResourceImported->LODModels.Num(); ++LODIdx)
//			{
//
//				FSkeletalMeshLODModel& LODModel = ResourceImported->LODModels[LODIdx];
//				FSkeletalMeshImportData RawMesh;
//
//				Mesh->LoadLODImportedData(LODIdx, RawMesh);
//
//				for (const FString& MorphTarget : MorphTargets)
//				{
//					int32 ImportMorphIndex = RawMesh.MorphTargetNames.IndexOfByKey(MorphTarget);
//					if (ImportMorphIndex != INDEX_NONE)
//					{
//						if (RawMesh.MorphTargets.IsValidIndex(ImportMorphIndex))
//						{
//							RawMesh.MorphTargets.RemoveAt(ImportMorphIndex);
//						}
//						if (RawMesh.MorphTargetNames.IsValidIndex(ImportMorphIndex))
//						{
//							RawMesh.MorphTargetNames.RemoveAt(ImportMorphIndex);
//						}
//						if (RawMesh.MorphTargetModifiedPoints.IsValidIndex(ImportMorphIndex))
//						{
//							RawMesh.MorphTargetModifiedPoints.RemoveAt(ImportMorphIndex);
//						}
//					}
//				}
//
//				Mesh->SaveLODImportedData(LODIdx, RawMesh);
//			}
//		}
//
//		bool bMorphTargetObjRemoved = false;
//
//#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27) || ENGINE_MAJOR_VERSION == 5
//		TArray<UMorphTarget*>& LocalMorphTargets = Mesh->GetMorphTargets();
//#else
//		TArray<UMorphTarget*>& LocalMorphTargets = Mesh->MorphTargets;
//#endif
//
//		for (const FString& MorphTarget : MorphTargets)
//		{
//			UMorphTarget* MorphTargetObj = FindMorphTarget(Mesh, MorphTarget);
//			if (MorphTargetObj)
//			{
//				bMorphTargetObjRemoved = true;
//				LocalMorphTargets.Remove(MorphTargetObj);
//			}
//		}
//
//		if (bMorphTargetObjRemoved && bInvalidateRenderData)
//		{
//			Mesh->InitMorphTargetsAndRebuildRenderData();
//		}
//
//		Mesh->MarkPackageDirty();
//	}
}

void UMeshEdittingLibrary::ApplyMorphTargetToImportData(USkeletalMesh* Mesh, FString MorphTargetName, const TArray<FMorphTargetDelta>& Deltas, bool bInvalidateRenderData)
{
	/*Mesh->Modify();
	Mesh->InvalidateDeriveDataCacheGUID();
	ApplyMorphTargetToImportData(Mesh, MorphTargetName, Deltas, 0);
	ApplyMorphTargetToLODs(Mesh, MorphTargetName, Deltas);
	if (bInvalidateRenderData)
	{
		Mesh->InitMorphTargetsAndRebuildRenderData();
	}
	Mesh->MarkPackageDirty();*/
}

void UMeshEdittingLibrary::ApplyMorphTargetToImportData(USkeletalMesh* Mesh, FString MorphTargetName, const TArray<FMorphTargetDelta>& Deltas, int32 LOD)
{
	/*if (Mesh && Deltas.Num() > 0)
	{
		FSkeletalMeshModel* ResourceImported = Mesh->GetImportedModel();
		if (ResourceImported)
		{
			if (ResourceImported->LODModels.IsValidIndex(LOD))
			{
				FSkeletalMeshLODModel& LODModel = ResourceImported->LODModels[LOD];
				FSkeletalMeshImportData RawMesh;

				Mesh->LoadLODImportedData(LOD, RawMesh);

				FSkeletalMeshImportData* ImportMorph = nullptr;
				TSet<uint32>* ModifiedPoints = nullptr;
				int32 ImportMorphIndex = RawMesh.MorphTargetNames.IndexOfByKey(MorphTargetName);
				if (ImportMorphIndex != INDEX_NONE)
				{
					ImportMorph = &RawMesh.MorphTargets[ImportMorphIndex];
					ModifiedPoints = &RawMesh.MorphTargetModifiedPoints[ImportMorphIndex];
					ImportMorph->Points.Empty();
					ModifiedPoints->Empty();
				}
				else {
					RawMesh.MorphTargetNames.Add(MorphTargetName);
					ImportMorph = &RawMesh.MorphTargets.AddDefaulted_GetRef();
					ModifiedPoints = &RawMesh.MorphTargetModifiedPoints.AddDefaulted_GetRef();
				}

				TMap<int32, FMorphTargetDelta> MorphVerticesMap;

				if (RawMesh.Points.Num())
				{
					for (const auto& Delta : Deltas)
					{
						if (LODModel.MeshToImportVertexMap.IsValidIndex(Delta.SourceIdx))
						{
							const int32 RawIndex = LODModel.MeshToImportVertexMap[Delta.SourceIdx];
							if (RawMesh.Points.IsValidIndex(RawIndex))
							{
								MorphVerticesMap.Add(RawIndex, Delta);
							}
						}
					}

					FCriticalSection Lock;
					ParallelFor(RawMesh.Points.Num(), [&](int32 Idx)
						{
							if (MorphVerticesMap.Contains(Idx))
							{
								Lock.Lock();
								ImportMorph->Points.Add(RawMesh.Points[Idx] + MorphVerticesMap[Idx].PositionDelta);
								ModifiedPoints->Add(Idx);
								Lock.Unlock();
							}
						});

					Mesh->SaveLODImportedData(LOD, RawMesh);
				}


				UMorphTarget* MorphTargetObj = FindMorphTarget(Mesh, MorphTargetName);
				if (!MorphTargetObj)
				{
					CreateMorphTargetObj(Mesh, MorphTargetName, false);

				}

				MorphTargetObj = FindMorphTarget(Mesh, MorphTargetName);

				if (MorphTargetObj)
				{
					MorphTargetObj->PopulateDeltas(Deltas, LOD, LODModel.Sections, false, false);
					if (!MorphTargetObj->HasDataForLOD(LOD))
					{
						CreateEmptyLODModel(MorphTargetObj->MorphLODModels[LOD]);
					}
				}
			}
		}
	}*/
}

void UMeshEdittingLibrary::GetMorphTargetDeltas(USkeletalMesh* Mesh, FString MorphTargetName, TArray<FMorphTargetDelta>& Deltas, int32 LOD)
{
	/*Deltas.Empty();
	if (Mesh)
	{
		FSkeletalMeshModel* ResourceImported = Mesh->GetImportedModel();
		if (ResourceImported)
		{
			if (ResourceImported->LODModels.IsValidIndex(LOD))
			{
				FSkeletalMeshLODModel& LODModel = ResourceImported->LODModels[LOD];
				bool bIsLODImportedDataBuildAvailable = false;
				bIsLODImportedDataBuildAvailable = Mesh->IsLODImportedDataBuildAvailable(LOD);

				if (bIsLODImportedDataBuildAvailable)
				{
					FSkeletalMeshImportData RawMesh;
					Mesh->LoadLODImportedData(LOD, RawMesh);

					const int32 ImportMorphIndex = RawMesh.MorphTargetNames.IndexOfByKey(MorphTargetName);
					int32 ImportedModified = 0;
					if (ImportMorphIndex != INDEX_NONE)
					{
						const FSkeletalMeshImportData& ImportMorph = RawMesh.MorphTargets[ImportMorphIndex];
						const TSet<uint32>& ModifiedPoints = RawMesh.MorphTargetModifiedPoints[ImportMorphIndex];

						int32 CurrentPointIdx = 0;

						for (const uint32& Idx : ModifiedPoints)
						{
							const int32 MeshIndex = LODModel.MeshToImportVertexMap.Find(Idx);
							if (MeshIndex != INDEX_NONE)
							{
								FMorphTargetDelta NewDelta;
								NewDelta.SourceIdx = MeshIndex;
								NewDelta.PositionDelta = ImportMorph.Points[CurrentPointIdx] - RawMesh.Points[Idx];
								Deltas.Add(NewDelta);
							}
							CurrentPointIdx++;
						}
					}
				}
				else {
					UMorphTarget* MorphTargetObj = FindMorphTarget(Mesh, MorphTargetName);
					if (MorphTargetObj)
					{
						GetMorphTargetDeltas(Mesh, MorphTargetObj, Deltas, LOD);
					}
				}
			}
		}
	}*/
}

void UMeshEdittingLibrary::GetMorphTargetDeltas(USkeletalMesh* Mesh, UMorphTarget* MorphTarget, TArray<FMorphTargetDelta>& Deltas, int32 LOD)
{
	/*if (Mesh && MorphTarget)
	{
		if (MorphTarget->HasDataForLOD(LOD))
		{
			Deltas = MorphTarget->MorphLODModels[LOD].Vertices;
		}
	}*/
}

void UMeshEdittingLibrary::GetMorphTargetNames(USkeletalMesh* Mesh, TArray<FString>& MorphTargets)
{
//	if (Mesh)
//	{
//		FSkeletalMeshModel* ResourceImported = Mesh->GetImportedModel();
//		if (ResourceImported)
//		{
//			if (ResourceImported->LODModels.IsValidIndex(0))
//			{
//				FSkeletalMeshLODModel& LODModel = ResourceImported->LODModels[0];
//				FSkeletalMeshImportData RawMesh;
//				Mesh->LoadLODImportedData(0, RawMesh);
//				MorphTargets = RawMesh.MorphTargetNames;
//			}
//		}
//
//#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27) || ENGINE_MAJOR_VERSION == 5
//		TArray<UMorphTarget*>& LocalMorphTargets = Mesh->GetMorphTargets();
//#else
//		TArray<UMorphTarget*>& LocalMorphTargets = Mesh->MorphTargets;
//#endif
//		for (UMorphTarget* MorphTarget : LocalMorphTargets)
//		{
//			if (MorphTarget)
//			{
//				MorphTargets.AddUnique(MorphTarget->GetName());
//			}
//		}
//
//	}
}

void UMeshEdittingLibrary::SetEnableBuildData(USkeletalMesh* Mesh, bool NewValue)
{
	/*if (Mesh)
	{
		FSkeletalMeshModel* ResourceImported = Mesh->GetImportedModel();
		Mesh->Modify();

		Mesh->InvalidateDeriveDataCacheGUID();

		if (ResourceImported)
		{
			for (int32 LODIdx = 0; LODIdx < Mesh->GetLODNum(); ++LODIdx)
			{
				if (!Mesh->IsLODImportedDataEmpty(LODIdx))
				{
					if (!NewValue)
					{
						Mesh->SetLODImportedDataVersions(LODIdx, ESkeletalMeshGeoImportVersions::Before_Versionning, ESkeletalMeshSkinningImportVersions::Before_Versionning);
					}
					else {
						Mesh->SetLODImportedDataVersions(LODIdx, ESkeletalMeshGeoImportVersions::LatestVersion, ESkeletalMeshSkinningImportVersions::LatestVersion);
					}
				}
			}
		}
		Mesh->MarkPackageDirty();

		UMeshEdittingLibrary::SaveSkeletalMesh(Mesh);
	}*/
}


void UMeshEdittingLibrary::ApplyMorphTargetToLODs(USkeletalMesh* Mesh, FString MorphTargetName, const TArray<FMorphTargetDelta>& Deltas)
{
	/*checkf(Mesh, TEXT("Invalid skeletal mesh."));

	UCharacterCreationSettings* Settings = GetMutableDefault<UCharacterCreationSettings>();
	UE::Geometry::FDynamicMesh3 OriginalMesh;

	if (Settings && !Settings->bUseUVProjectionForLODs)
	{
		SkeletalMeshToDynamicMesh(Mesh, OriginalMesh);
	}

	if (Deltas.Num() > 0)
	{
		for (int32 CurrentLOD = 0; CurrentLOD < Mesh->GetImportedModel()->LODModels.Num(); ++CurrentLOD)
		{
			if (CurrentLOD > 0)
			{
				TArray<FMorphTargetDelta> LODDeltas;
				if (Settings)
				{
					if (Settings->bRemapMorphTargets)
					{
						FSkeletalMeshLODInfo* SrcLODInfo = Mesh->GetLODInfo(CurrentLOD);
						if (SrcLODInfo)
						{
							SrcLODInfo->ReductionSettings.bRemapMorphTargets = true;
						}
					}


					if (Settings->bUseUVProjectionForLODs)
					{
						ApplyMorphTargetToLOD(Mesh, Deltas, 0, CurrentLOD, LODDeltas);
					}
					else
					{
						UE::Geometry::FDynamicMesh3 LODMesh;
						SkeletalMeshToDynamicMesh(Mesh, LODMesh, NULL, TArray<FFinalSkinVertex>(), CurrentLOD);
						ApplySourceDeltasToDynamicMesh(OriginalMesh, LODMesh, Deltas, LODDeltas, Settings->Threshold, 1.0f, Settings->SmoothIterations, Settings->SmoothStrength, false);
					}
				}
				else {
					ApplyMorphTargetToLOD(Mesh, Deltas, 0, CurrentLOD, LODDeltas);
				}

				ApplyMorphTargetToImportData(Mesh, MorphTargetName, LODDeltas, CurrentLOD);
			}
		}
	}*/
}

bool UMeshEdittingLibrary::ApplyMorphTargetsToSkeletalMesh(USkeletalMesh* SkeletalMesh, const TArray< TSharedPtr<FMeshMorpherMorphTargetInfo> >& MorphTargets)
{
	/*SkeletalMesh->WaitForPendingInitOrStreaming();

	if (SkeletalMesh && MorphTargets.Num() > 0)
	{
		GWarn->BeginSlowTask(FText::FromString("Bake Morph Target(s)"), true);
		GWarn->UpdateProgress(0, 7);
		SkeletalMesh->Modify();
		SkeletalMesh->GetImportedModel();
		FSkeletalMeshModel* Resource = SkeletalMesh->GetImportedModel();
		check(Resource);

		GWarn->StatusForceUpdate(1, 7, FText::FromString("Convert Skeletal Mesh to Dynamic Mesh..."));
		TArray<UE::Geometry::FDynamicMesh3> DynamicMeshes;
		DynamicMeshes.SetNum(Resource->LODModels.Num());
		for (int32 LOD = 0; LOD < Resource->LODModels.Num(); LOD++)
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("LOD"), FText::FromString(FString::FromInt(LOD)));
			const FText StatusUpdate = FText::Format(LOCTEXT("ConvertSkeletalMesh", "Convert Skeletal Mesh to Dynamic Mesh for LOD ({Deltas})"), Args);
			GWarn->StatusForceUpdate(2, 7, StatusUpdate);
			SkeletalMeshToDynamicMesh(SkeletalMesh, DynamicMeshes[LOD], NULL, TArray<FFinalSkinVertex>(), LOD);
		}


		GWarn->StatusForceUpdate(3, 7, FText::FromString("Load Existing Morph Target(s}"));
		TMap<int32, TArray<TArray<FMorphTargetDelta>>> Deltas;
		for (int32 MorphTargetIdx = 0; MorphTargetIdx < MorphTargets.Num(); ++MorphTargetIdx)
		{
			if (MorphTargets[MorphTargetIdx].IsValid())
			{
				FFormatNamedArguments Args;
				Args.Add(TEXT("MorphTarget"), FText::FromString(MorphTargets[MorphTargetIdx]->Name.ToString()));
				const FText StatusUpdate = FText::Format(LOCTEXT("LoadExistingMorphTarget", "Load Morph Target: ({MorphTarget})"), Args);
				GWarn->StatusForceUpdate(3, 7, StatusUpdate);
				TArray <TArray<FMorphTargetDelta>>& MorphTargetDelta = Deltas.FindOrAdd(MorphTargetIdx);
				for (int32 LOD = 0; LOD < Resource->LODModels.Num(); LOD++)
				{
					TArray<FMorphTargetDelta>& LODDeltas = MorphTargetDelta.AddDefaulted_GetRef();
					GetMorphTargetDeltas(SkeletalMesh, MorphTargets[MorphTargetIdx]->Name.ToString(), LODDeltas, LOD);
				}
			}
		}

		TArray<int32> SkipMorphTargetIdx;
		GWarn->StatusForceUpdate(4, 7, FText::FromString("Baking Selected Morph Target(s}"));
		for (int32 MorphTargetIdx = 0; MorphTargetIdx < MorphTargets.Num(); ++MorphTargetIdx)
		{
			if (MorphTargets[MorphTargetIdx].IsValid() && !FMath::IsNearlyZero(MorphTargets[MorphTargetIdx]->Weight))
			{

				FFormatNamedArguments Args;
				Args.Add(TEXT("MorphTarget"), FText::FromString(MorphTargets[MorphTargetIdx]->Name.ToString()));
				const FText StatusUpdate = FText::Format(LOCTEXT("BakeMorphTarget", "Baking Morph Target: ({MorphTarget})"), Args);
				GWarn->StatusForceUpdate(4, 7, StatusUpdate);

				if (MorphTargets[MorphTargetIdx]->bRemove)
				{
					SkipMorphTargetIdx.Add(MorphTargetIdx);
				}
				TArray <TArray<FMorphTargetDelta>>& MorphTargetDelta = Deltas[MorphTargetIdx];
				for (int32 LOD = 0; LOD < Resource->LODModels.Num(); LOD++)
				{
					TArray<FMorphTargetDelta>& LODDeltas = MorphTargetDelta[LOD];
					GetMorphTargetDeltas(SkeletalMesh, MorphTargets[MorphTargetIdx]->Name.ToString(), LODDeltas, LOD);
					if (LODDeltas.Num())
					{
						UE::Geometry::FDynamicMesh3 DynamicMesh = DynamicMeshes[LOD];
						ParallelFor(LODDeltas.Num(), [&](int32 Index)
							{
								const FMorphTargetDelta& Key = LODDeltas[Index];
								if (DynamicMesh.IsVertex(Key.SourceIdx))
								{
									FVector3d CurrentPosition = DynamicMesh.GetVertex(Key.SourceIdx) + Key.PositionDelta * MorphTargets[MorphTargetIdx]->Weight;
									DynamicMesh.SetVertex(Key.SourceIdx, CurrentPosition);
								}
							});

						FSkeletalMeshLODModel& LODModel = Resource->LODModels[LOD];
						FSkeletalMeshImportData RawImportDataMesh;
						SkeletalMesh->LoadLODImportedData(LOD, RawImportDataMesh);
						bool bSaveImportData = false;

						for (const FMorphTargetDelta& MorphKey : LODDeltas)
						{
							if (DynamicMesh.IsVertex(MorphKey.SourceIdx))
							{
								const FVector3d NewNormal = FMeshNormals::ComputeVertexNormal(DynamicMesh, MorphKey.SourceIdx);

								if (LODModel.MeshToImportVertexMap.IsValidIndex(MorphKey.SourceIdx))
								{
									const int32 RawIndex = LODModel.MeshToImportVertexMap[MorphKey.SourceIdx];
									if (RawImportDataMesh.Points.IsValidIndex(RawIndex))
									{
										bSaveImportData = true;
										RawImportDataMesh.Points[RawIndex] += MorphKey.PositionDelta * MorphTargets[MorphTargetIdx]->Weight;
									}
								}

								int32 SectionIndex;
								int32 Idx;
								LODModel.GetSectionFromVertexIndex(MorphKey.SourceIdx, SectionIndex, Idx);

								FSoftSkinVertex& DestVertex = LODModel.Sections[SectionIndex].SoftVertices[Idx];
								DestVertex.Position += MorphKey.PositionDelta * MorphTargets[MorphTargetIdx]->Weight;
								uint8 W = DestVertex.TangentZ.W;
								DestVertex.TangentZ = FVector(NewNormal);
								DestVertex.TangentZ.W = W;

							}
						}

						if (bSaveImportData)
						{
							SkeletalMesh->SaveLODImportedData(LOD, RawImportDataMesh);
						}
					}
				}
			}
		}


		GWarn->StatusForceUpdate(5, 7, FText::FromString("Ofsetting Morph Target(s}..."));
		for (int32 MorphTargetIdx = 0; MorphTargetIdx < MorphTargets.Num(); ++MorphTargetIdx)
		{
			if (MorphTargets[MorphTargetIdx].IsValid() && !SkipMorphTargetIdx.Contains(MorphTargetIdx))
			{
				FFormatNamedArguments Args;
				Args.Add(TEXT("MorphTarget"), FText::FromString(MorphTargets[MorphTargetIdx]->Name.ToString()));
				const FText StatusUpdate = FText::Format(LOCTEXT("OfsetMorphTarget", "Ofsetting Morph Target: ({MorphTarget})"), Args);
				GWarn->StatusForceUpdate(4, 4, StatusUpdate);

				TArray <TArray<FMorphTargetDelta>>& MorphTargetDelta = Deltas[MorphTargetIdx];
				for (int32 LOD = 0; LOD < Resource->LODModels.Num(); LOD++)
				{
					FSkeletalMeshLODModel& LODModUnreel = Resource->LODModels[LOD];
					TArray<FMorphTargetDelta>& LODDeltas = MorphTargetDelta[LOD];
					ApplyMorphTargetToImportData(SkeletalMesh, MorphTargets[MorphTargetIdx]->Name.ToString(), LODDeltas, LOD);

				}
			}
		}


		UCharacterCreationSettings* Settings = GetMutableDefault<UCharacterCreationSettings>();
		if (Settings && Settings->bRemapMorphTargets)
		{
			for (int32 LOD = 0; LOD < Resource->LODModels.Num(); LOD++)
			{

				FSkeletalMeshLODInfo* SrcLODInfo = SkeletalMesh->GetLODInfo(LOD);
				if (SrcLODInfo)
				{
					SrcLODInfo->ReductionSettings.bRemapMorphTargets = true;
				}
			}
		}

		TArray<FString> MorphTargetNames;

		GWarn->StatusForceUpdate(6, 7, FText::FromString("Removing selected Morph Target(s}..."));
		for (const TSharedPtr<FMeshMorpherMorphTargetInfo>& MorphTarget : MorphTargets)
		{
			if (MorphTarget.IsValid() && MorphTarget->bRemove)
			{
				MorphTargetNames.Add(MorphTarget->Name.ToString());
			}
		}
		RemoveMorphTargetsFromImportData(SkeletalMesh, MorphTargetNames, false);

		GWarn->StatusForceUpdate(7, 7, FText::FromString("Initializing Morph Targets and Rebuilding Render Data..."));
		SkeletalMesh->InvalidateDeriveDataCacheGUID();
		SkeletalMesh->InitMorphTargetsAndRebuildRenderData();
		SkeletalMesh->MarkPackageDirty();
		GWarn->EndSlowTask();
		return true;

	}*/

	return false;

}

void UMeshEdittingLibrary::SaveSkeletalMesh(USkeletalMesh* Mesh)
{
	/*checkf(Mesh, TEXT("Invalid skeletal mesh."));
	Mesh->InitMorphTargetsAndRebuildRenderData();

	TArray< UPackage*> Packages;

	UPackage* SkeletalMeshPkg = Cast<UPackage>(Mesh->GetOuter());
	if (SkeletalMeshPkg)
	{
		Packages.Add(SkeletalMeshPkg);
	}

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27) || ENGINE_MAJOR_VERSION == 5
	USkeleton* Skeleton = Mesh->GetSkeleton();
#else
	USkeleton* Skeleton = Mesh->Skeleton;
#endif

	if (Skeleton)
	{
		UPackage* SkeletonPkg = Cast<UPackage>(Skeleton->GetOuter());
		if (SkeletonPkg)
		{
			Packages.Add(SkeletonPkg);
		}
	}
	FEditorFileUtils::PromptForCheckoutAndSave(Packages, true, true, nullptr, false, true);*/
}

void UMeshEdittingLibrary::ExportMorphTargetToStaticMesh(FString MorphTargetName, const FMeshDescription& MeshDescription, const TArray<FStaticMaterial>& StaticMaterials)
{
//	FString PackageName = FString(TEXT("/Game/Meshes/")) + MorphTargetName;
//	FString Name;
//	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
//	AssetToolsModule.Get().CreateUniqueAssetName(PackageName, TEXT(""), PackageName, Name);
//
//	TSharedPtr<SDlgPickAssetPath> PickAssetPathWidget =
//		SNew(SDlgPickAssetPath)
//		.Title(FText::FromString(FString("Choose New StaticMesh Location")))
//		.DefaultAssetPath(FText::FromString(PackageName));
//
//	if (PickAssetPathWidget->ShowModal() == EAppReturnType::Ok)
//	{
//		// Get the full name of where we want to create the physics asset.
//		FString UserPackageName = PickAssetPathWidget->GetFullAssetPath().ToString();
//		FName MeshName(*FPackageName::GetLongPackageAssetName(UserPackageName));
//
//		// Check if the user inputed a valid asset name, if they did not, give it the generated default name
//		if (MeshName == NAME_None)
//		{
//			// Use the defaults that were already generated.
//			UserPackageName = PackageName;
//			MeshName = *Name;
//		}
//
//		// If we got some valid data.
//		if (MeshDescription.Polygons().Num() > 0)
//		{
//			// Then find/create it.
//			UPackage* Package = CreatePackage(*UserPackageName);
//			check(Package);
//
//			// Create StaticMesh object
//			UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Package, MeshName, RF_Public | RF_Standalone);
//			StaticMesh->InitResources();
//
//#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27) || ENGINE_MAJOR_VERSION == 5
//			StaticMesh->SetLightingGuid(FGuid::NewGuid());
//#else
//			StaticMesh->LightingGuid = FGuid::NewGuid();
//#endif
//
//			// Add source to new StaticMesh
//			FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
//			SrcModel.BuildSettings.bRecomputeNormals = true;
//			SrcModel.BuildSettings.bRecomputeTangents = true;
//			SrcModel.BuildSettings.bRemoveDegenerates = true;
//			SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
//			SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
//			SrcModel.BuildSettings.bGenerateLightmapUVs = true;
//			SrcModel.BuildSettings.SrcLightmapIndex = 0;
//			SrcModel.BuildSettings.DstLightmapIndex = 1;
//			StaticMesh->CreateMeshDescription(0, MeshDescription);
//			StaticMesh->CommitMeshDescription(0);
//
//#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27) || ENGINE_MAJOR_VERSION == 5
//			StaticMesh->SetStaticMaterials(StaticMaterials);
//#else
//			StaticMesh->StaticMaterials = StaticMaterials;
//#endif
//
//			//Set the Imported version before calling the build
//			StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;
//
//			// Build mesh from source
//			StaticMesh->Build(false);
//			StaticMesh->PostEditChange();
//
//			// Notify asset registry of new asset
//			FAssetRegistryModule::AssetCreated(StaticMesh);
//
//		}
//	}
}

void UMeshEdittingLibrary::ExportMorphTargetToStandAlone(USkeletalMesh* Mesh, FString MorphTargetName)
{
	//if (Mesh)
	//{
	//	FString PackageName = FString(TEXT("/Game/MorphTargets/")) + MorphTargetName;
	//	FString Name;
	//	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	//	AssetToolsModule.Get().CreateUniqueAssetName(PackageName, TEXT(""), PackageName, Name);

	//	TSharedPtr<SDlgPickAssetPath> PickAssetPathWidget =
	//		SNew(SDlgPickAssetPath)
	//		.Title(FText::FromString(FString("Choose New Morph Target Location")))
	//		.DefaultAssetPath(FText::FromString(PackageName));
	//	if (PickAssetPathWidget->ShowModal() == EAppReturnType::Ok)
	//	{
	//		FString UserPackageName = PickAssetPathWidget->GetFullAssetPath().ToString();
	//		FName MorphTarget(*FPackageName::GetLongPackageAssetName(UserPackageName));

	//		// Check if the user inputed a valid asset name, if they did not, give it the generated default name
	//		if (MorphTarget == NAME_None)
	//		{
	//			// Use the defaults that were already generated.
	//			UserPackageName = PackageName;
	//			MorphTarget = *Name;
	//		}

	//		// Then find/create it.
	//		UPackage* Package = CreatePackage(*UserPackageName);
	//		check(Package);

	//		// Create MorphTarget object
	//		UBAMorphTarget* NewMorphTarget = NewObject<UBAMorphTarget>(Package, MorphTarget, RF_Public | RF_Standalone);

	//		NewMorphTarget->BaseSkelMesh = Mesh;

	//		TArray<FMorphTargetDelta> Deltas;
	//		GetMorphTargetDeltas(Mesh, MorphTargetName, Deltas);


	//		UCharacterCreationSettings* Settings = GetMutableDefault<UCharacterCreationSettings>();
	//		UE::Geometry::FDynamicMesh3 OriginalMesh;

	//		if (Settings && !Settings->bUseUVProjectionForLODs)
	//		{
	//			SkeletalMeshToDynamicMesh(Mesh, OriginalMesh);
	//		}

	//		if (Deltas.Num() > 0)
	//		{
	//			for (int32 CurrentLOD = 0; CurrentLOD < Mesh->GetImportedModel()->LODModels.Num(); ++CurrentLOD)
	//			{
	//				if (CurrentLOD > 0)
	//				{
	//					TArray<FMorphTargetDelta> LODDeltas;
	//					if (Settings)
	//					{
	//						if (Settings->bUseUVProjectionForLODs)
	//						{
	//							ApplyMorphTargetToLOD(Mesh, Deltas, 0, CurrentLOD, LODDeltas);
	//						}
	//						else
	//						{
	//							UE::Geometry::FDynamicMesh3 LODMesh;
	//							SkeletalMeshToDynamicMesh(Mesh, LODMesh, NULL, TArray<FFinalSkinVertex>(), CurrentLOD);
	//							ApplySourceDeltasToDynamicMesh(OriginalMesh, LODMesh, Deltas, LODDeltas, Settings->Threshold, 1.0f, Settings->SmoothIterations, Settings->SmoothStrength, false);
	//						}
	//					}
	//					else {
	//						ApplyMorphTargetToLOD(Mesh, Deltas, 0, CurrentLOD, LODDeltas);
	//					}

	//					if (LODDeltas.Num() > 0)
	//					{
	//						NewMorphTarget->PopulateDeltas(LODDeltas, CurrentLOD, Mesh->GetImportedModel()->LODModels[CurrentLOD].Sections);
	//					}
	//				}
	//				else {
	//					NewMorphTarget->PopulateDeltas(Deltas, CurrentLOD, Mesh->GetImportedModel()->LODModels[CurrentLOD].Sections);
	//				}
	//			}
	//		}

	//		NewMorphTarget->PostLoad();
	//		NewMorphTarget->MarkPackageDirty();
	//		// Notify asset registry of new asset
	//		FAssetRegistryModule::AssetCreated(NewMorphTarget);

	//	}
	//}
}


/**
	Below code is taken from LODUtilities but slightly modifed.
*/

struct FMMTargetMatch
{
	float BarycentricWeight[3]; //The weight we use to interpolate the TARGET data
	uint32 Indices[3]; //BASE Index of the triangle vertice
};

struct FMMTriangleElement
{
	FBox2D UVsBound;
	FBox PositionBound;
	TArray<FSoftSkinVertex> Vertices;
	TArray<uint32> Indexes;
	uint32 TriangleIndex;
};

/** Given three direction vectors, indicates if A and B are on the same 'side' of Vec. */
bool VectorsOnSameSide(const FVector2D& Vec, const FVector2D& A, const FVector2D& B)
{
	return !FMath::IsNegativeOrNegativeZero(((B.Y - A.Y) * (Vec.X - A.X)) + ((A.X - B.X) * (Vec.Y - A.Y)));
}

float PointToSegmentDistanceSquare(const FVector2D& A, const FVector2D& B, const FVector2D& P)
{
	return FVector2D::DistSquared(P, FMath::ClosestPointOnSegment2D(P, A, B));
}

/** Return true if P is within triangle created by A, B and C. */
bool PointInTriangle(const FVector2D& A, const FVector2D& B, const FVector2D& C, const FVector2D& P)
{
	////If the point is on a triangle point we consider the point inside the triangle

	//if (P.Equals(A) || P.Equals(B) || P.Equals(C))
	//{
	//	return true;
	//}
	//// If its on the same side as the remaining vert for all edges, then its inside.	
	//if (VectorsOnSameSide(A, B, P) &&
	//	VectorsOnSameSide(B, C, P) &&
	//	VectorsOnSameSide(C, A, P))
	//{
	//	return true;
	//}

	////Make sure point on the edge are count inside the triangle
	//if (PointToSegmentDistanceSquare(A, B, P) <= KINDA_SMALL_NUMBER)
	//{
	//	return true;
	//}
	//if (PointToSegmentDistanceSquare(B, C, P) <= KINDA_SMALL_NUMBER)
	//{
	//	return true;
	//}
	//if (PointToSegmentDistanceSquare(C, A, P) <= KINDA_SMALL_NUMBER)
	//{
	//	return true;
	//}
	return false;
}

/** Given three direction vectors, indicates if A and B are on the same 'side' of Vec. */
bool VectorsOnSameSide(const FVector& Vec, const FVector& A, const FVector& B, const float SameSideDotProductEpsilon)
{
	const FVector CrossA = Vec ^ A;
	const FVector CrossB = Vec ^ B;
	float DotWithEpsilon = SameSideDotProductEpsilon + (CrossA | CrossB);
	return !FMath::IsNegativeOrNegativeZero(DotWithEpsilon);
}

/** Util to see if P lies within triangle created by A, B and C. */
bool PointInTriangle(const FVector& A, const FVector& B, const FVector& C, const FVector& P)
{
	// Cross product indicates which 'side' of the vector the point is on
	// If its on the same side as the remaining vert for all edges, then its inside.	
	if (VectorsOnSameSide(B - A, P - A, C - A, KINDA_SMALL_NUMBER) &&
		VectorsOnSameSide(C - B, P - B, A - B, KINDA_SMALL_NUMBER) &&
		VectorsOnSameSide(A - C, P - C, B - C, KINDA_SMALL_NUMBER))
	{
		return true;
	}
	return false;
}

FVector GetBaryCentric(const FVector& Point, const FVector& A, const FVector& B, const FVector& C)
{
	// Compute the normal of the triangle
	const FVector TriNorm = (B - A) ^ (C - A);

	//check collinearity of A,B,C
	if (TriNorm.SizeSquared() <= SMALL_NUMBER)
	{
		float DistA = FVector::DistSquared(Point, A);
		float DistB = FVector::DistSquared(Point, B);
		float DistC = FVector::DistSquared(Point, C);
		if (DistA <= DistB && DistA <= DistC)
		{
			return FVector(1.0f, 0.0f, 0.0f);
		}
		if (DistB <= DistC)
		{
			return FVector(0.0f, 1.0f, 0.0f);
		}
		return FVector(0.0f, 0.0f, 1.0f);
	}
	return FMath::ComputeBaryCentric2D(Point, A, B, C);
}

bool FindTriangleUVMatch(const FVector2D& TargetUV, const TArray<FMMTriangleElement>& Triangles, const TArray<uint32>& QuadTreeTriangleResults, TArray<uint32>& MatchTriangleIndexes)
{
	for (uint32 TriangleIndex : QuadTreeTriangleResults)
	{
		const FMMTriangleElement& TriangleElement = Triangles[TriangleIndex];
		//if (PointInTriangle(TriangleElement.Vertices[0].UVs[0], TriangleElement.Vertices[1].UVs[0], TriangleElement.Vertices[2].UVs[0], TargetUV))
		//{
		//	MatchTriangleIndexes.Add(TriangleIndex);
		//}
		//TriangleIndex++;
	}
	return MatchTriangleIndexes.Num() == 0 ? false : true;
}

void ProjectTargetOnBase(const TArray<FSoftSkinVertex>& BaseVertices, const TArray<TArray<uint32>>& PerSectionBaseTriangleIndices,
	TArray<FMMTargetMatch>& TargetMatchData, const TArray<FSkelMeshSection>& TargetSections, const TArray<int32>& TargetSectionMatchBaseIndex, const TCHAR* DebugContext, float InDistanceThreshold = 0.05f, float InFailSafe = 0.1f)
{
	//bool bNoMatchMsgDone = false;
	//TArray<FMMTriangleElement> Triangles;
	////Project section target vertices on match base section using the UVs coordinates
	//for (int32 SectionIndex = 0; SectionIndex < TargetSections.Num(); ++SectionIndex)
	//{
	//	//Use the remap base index in case some sections disappear during the reduce phase
	//	int32 BaseSectionIndex = TargetSectionMatchBaseIndex[SectionIndex];
	//	if (BaseSectionIndex == INDEX_NONE || !PerSectionBaseTriangleIndices.IsValidIndex(BaseSectionIndex) || PerSectionBaseTriangleIndices[BaseSectionIndex].Num() < 1)
	//	{
	//		continue;
	//	}
	//	//Target vertices for the Section
	//	const TArray<FSoftSkinVertex>& TargetVertices = TargetSections[SectionIndex].SoftVertices;
	//	//Base Triangle indices for the matched base section
	//	const TArray<uint32>& BaseTriangleIndices = PerSectionBaseTriangleIndices[BaseSectionIndex];
	//	FBox2D BaseMeshUVBound(EForceInit::ForceInit);
	//	FBox BaseMeshPositionBound(EForceInit::ForceInit);
	//	//Fill the triangle element to speed up the triangle research
	//	Triangles.Reset(BaseTriangleIndices.Num() / 3);


	//	for (uint32 TriangleIndex = 0; TriangleIndex < (uint32)BaseTriangleIndices.Num(); TriangleIndex += 3)
	//	{
	//		FMMTriangleElement TriangleElement;
	//		TriangleElement.UVsBound.Init();
	//		for (int32 Corner = 0; Corner < 3; ++Corner)
	//		{
	//			uint32 CornerIndice = BaseTriangleIndices[TriangleIndex + Corner];
	//			check(BaseVertices.IsValidIndex(CornerIndice));
	//			const FSoftSkinVertex& BaseVertex = BaseVertices[CornerIndice];
	//			TriangleElement.Indexes.Add(CornerIndice);
	//			TriangleElement.Vertices.Add(BaseVertex);
	//			TriangleElement.UVsBound += BaseVertex.UVs[0];
	//			BaseMeshPositionBound += BaseVertex.Position;
	//		}
	//		BaseMeshUVBound += TriangleElement.UVsBound;
	//		TriangleElement.TriangleIndex = Triangles.Num();
	//		Triangles.Add(TriangleElement);
	//	}

	//	check(!BaseMeshUVBound.GetExtent().IsNearlyZero());
	//	//Setup the Quad tree
	//	float UVsQuadTreeMinSize = 0.001f;
	//	TQuadTree<uint32, 100> QuadTree(BaseMeshUVBound, UVsQuadTreeMinSize);
	//	for (FMMTriangleElement& TriangleElement : Triangles)
	//	{
	//		QuadTree.Insert(TriangleElement.TriangleIndex, TriangleElement.UVsBound, DebugContext);
	//	}
	//	//Retrieve all triangle that are close to our point, let get 5% of UV extend
	//	float DistanceThreshold = BaseMeshUVBound.GetExtent().Size() * InDistanceThreshold;
	//	//Find a match triangle for every target vertices

	//	ParallelFor(TargetVertices.Num(), [&](int32 TargetVertexIndex)
	//		{
	//			FVector2D TargetUV = TargetVertices[TargetVertexIndex].UVs[0];
	//			//Reset the last data without flushing the memmery allocation

	//			TArray<uint32> QuadTreeTriangleResults;
	//			QuadTreeTriangleResults.Reserve(Triangles.Num() / 10); //Reserve 10% to speed up the query

	//			const uint32 FullTargetIndex = TargetSections[SectionIndex].BaseVertexIndex + TargetVertexIndex;
	//			//Make sure the array is allocate properly
	//			if (TargetMatchData.IsValidIndex(FullTargetIndex))
	//			{
	//				//Set default data for the target match, in case we cannot found a match
	//				FMMTargetMatch& TargetMatch = TargetMatchData[FullTargetIndex];
	//				for (int32 Corner = 0; Corner < 3; ++Corner)
	//				{
	//					TargetMatch.Indices[Corner] = INDEX_NONE;
	//					TargetMatch.BarycentricWeight[Corner] = 0.3333f; //The weight will be use to found the proper delta
	//				}

	//				FVector2D Extent(DistanceThreshold, DistanceThreshold);
	//				FBox2D CurBox(TargetUV - Extent, TargetUV + Extent);
	//				while (QuadTreeTriangleResults.Num() <= 0)
	//				{
	//					QuadTree.GetElements(CurBox, QuadTreeTriangleResults);
	//					Extent *= 2;
	//					CurBox = FBox2D(TargetUV - Extent, TargetUV + Extent);
	//				}

	//				auto GetDistancePointToBaseTriangle = [&Triangles, &TargetVertices, &TargetVertexIndex](const uint32 BaseTriangleIndex)->float
	//				{
	//					FMMTriangleElement& CandidateTriangle = Triangles[BaseTriangleIndex];
	//					return FVector::DistSquared(FMath::ClosestPointOnTriangleToPoint(TargetVertices[TargetVertexIndex].Position, CandidateTriangle.Vertices[0].Position, CandidateTriangle.Vertices[1].Position, CandidateTriangle.Vertices[2].Position), TargetVertices[TargetVertexIndex].Position);
	//				};

	//				auto FailSafeUnmatchVertex = [&GetDistancePointToBaseTriangle, &QuadTreeTriangleResults](uint32& OutIndexMatch)->bool
	//				{
	//					bool bFoundMatch = false;
	//					float ClosestTriangleDistSquared = MAX_flt;
	//					for (uint32 MatchTriangleIndex : QuadTreeTriangleResults)
	//					{
	//						float TriangleDistSquared = GetDistancePointToBaseTriangle(MatchTriangleIndex);
	//						if (TriangleDistSquared < ClosestTriangleDistSquared)
	//						{
	//							ClosestTriangleDistSquared = TriangleDistSquared;
	//							OutIndexMatch = MatchTriangleIndex;
	//							bFoundMatch = true;
	//						}
	//					}
	//					return bFoundMatch;
	//				};

	//				//Find all Triangles that contain the Target UV
	//				if (QuadTreeTriangleResults.Num() > 0)
	//				{
	//					TArray<uint32> MatchTriangleIndexes;
	//					uint32 FoundIndexMatch = INDEX_NONE;
	//					bool bFoundMatch = true;
	//					if (!FindTriangleUVMatch(TargetUV, Triangles, QuadTreeTriangleResults, MatchTriangleIndexes))
	//					{
	//						if (!FailSafeUnmatchVertex(FoundIndexMatch))
	//						{
	//							bFoundMatch = false;
	//						}
	//					}

	//					if (bFoundMatch)
	//					{
	//						float ClosestTriangleDistSquared = MAX_flt;
	//						if (MatchTriangleIndexes.Num() == 1)
	//						{
	//							//One match, this mean no mirror UVs simply take the single match
	//							FoundIndexMatch = MatchTriangleIndexes[0];
	//							ClosestTriangleDistSquared = GetDistancePointToBaseTriangle(FoundIndexMatch);
	//						}
	//						else
	//						{
	//							//Geometry can use mirror so the UVs are not unique. Use the closest match triangle to the point to find the best match
	//							for (uint32 MatchTriangleIndex : MatchTriangleIndexes)
	//							{
	//								float TriangleDistSquared = GetDistancePointToBaseTriangle(MatchTriangleIndex);
	//								if (TriangleDistSquared < ClosestTriangleDistSquared)
	//								{
	//									ClosestTriangleDistSquared = TriangleDistSquared;
	//									FoundIndexMatch = MatchTriangleIndex;
	//								}
	//							}
	//						}

	//						//FAIL SAFE, make sure we have a match that make sense
	//						//Use the mesh section geometry bound extent (10% of it) to validate we are close enough.
	//						if (ClosestTriangleDistSquared > BaseMeshPositionBound.GetExtent().SizeSquared() * InFailSafe)
	//						{
	//							//Executing fail safe, if the UVs are too much off because of the reduction, use the closest distance to polygons to find the match
	//							//This path is not optimize and should not happen often.
	//							FailSafeUnmatchVertex(FoundIndexMatch);
	//						}

	//						//We should always have a valid match at this point
	//						check(FoundIndexMatch != INDEX_NONE);
	//						FMMTriangleElement& BestTriangle = Triangles[FoundIndexMatch];
	//						//Found the surface area of the 3 barycentric triangles from the UVs
	//						FVector BarycentricWeight;
	//						BarycentricWeight = GetBaryCentric(FVector(TargetUV, 0.0f), FVector(BestTriangle.Vertices[0].UVs[0], 0.0f), FVector(BestTriangle.Vertices[1].UVs[0], 0.0f), FVector(BestTriangle.Vertices[2].UVs[0], 0.0f));
	//						//Fill the target match
	//						for (int32 Corner = 0; Corner < 3; ++Corner)
	//						{
	//							TargetMatch.Indices[Corner] = BestTriangle.Indexes[Corner];
	//							TargetMatch.BarycentricWeight[Corner] = BarycentricWeight[Corner]; //The weight will be use to found the proper delta
	//						}
	//					}
	//				}
	//			}
	//		});
	//}
}

bool CreateLODMorphTarget(const TArray<FMorphTargetDelta>& Deltas, const TMap<uint32, uint32>& PerMorphTargetBaseIndexToMorphTargetDelta, const TMap<uint32, TArray<uint32>>& BaseMorphIndexToTargetIndexList, const TArray<FSoftSkinVertex>& TargetVertices, const TArray<FMMTargetMatch>& TargetMatchData, TArray<FMorphTargetDelta>& OutDeltas)
{
	//if (Deltas.Num() > 0)
	//{
	//	OutDeltas.Empty();
	//	TSet<uint32> CreatedTargetIndex;
	//	TMap<FVector, TArray<uint32>> MorphTargetPerPosition;
	//	for (uint32 MorphDeltaIndex = 0; MorphDeltaIndex < (uint32)(Deltas.Num()); ++MorphDeltaIndex)
	//	{
	//		const FMorphTargetDelta& MorphDelta = Deltas[MorphDeltaIndex];
	//		const TArray<uint32>* TargetIndexesPtr = BaseMorphIndexToTargetIndexList.Find(MorphDelta.SourceIdx);
	//		if (TargetIndexesPtr == nullptr)
	//		{
	//			continue;
	//		}
	//		const TArray<uint32>& TargetIndexes = *TargetIndexesPtr;
	//		for (int32 MorphTargetIndex = 0; MorphTargetIndex < TargetIndexes.Num(); ++MorphTargetIndex)
	//		{
	//			uint32 TargetIndex = TargetIndexes[MorphTargetIndex];
	//			if (CreatedTargetIndex.Contains(TargetIndex))
	//			{
	//				continue;
	//			}
	//			CreatedTargetIndex.Add(TargetIndex);
	//			const FVector& SearchPosition = TargetVertices[TargetIndex].Position;
	//			FMorphTargetDelta MatchMorphDelta;
	//			MatchMorphDelta.SourceIdx = TargetIndex;

	//			const FMMTargetMatch& TargetMatch = TargetMatchData[TargetIndex];

	//			//Find the Position/tangent delta for the MatchMorphDelta using the barycentric weight
	//			MatchMorphDelta.PositionDelta = FVector(0.0f);
	//			MatchMorphDelta.TangentZDelta = FVector(0.0f);
	//			for (int32 Corner = 0; Corner < 3; ++Corner)
	//			{
	//				const uint32* BaseMorphTargetIndexPtr = PerMorphTargetBaseIndexToMorphTargetDelta.Find(TargetMatch.Indices[Corner]);
	//				if (BaseMorphTargetIndexPtr != nullptr && Deltas.IsValidIndex(*BaseMorphTargetIndexPtr))
	//				{
	//					const FMorphTargetDelta& BaseMorphTargetDelta = Deltas[*BaseMorphTargetIndexPtr];
	//					FVector BasePositionDelta = !BaseMorphTargetDelta.PositionDelta.ContainsNaN() ? BaseMorphTargetDelta.PositionDelta : FVector(0.0f);
	//					FVector BaseTangentZDelta = !BaseMorphTargetDelta.TangentZDelta.ContainsNaN() ? BaseMorphTargetDelta.TangentZDelta : FVector(0.0f);
	//					MatchMorphDelta.PositionDelta += BasePositionDelta * TargetMatch.BarycentricWeight[Corner];
	//					MatchMorphDelta.TangentZDelta += BaseTangentZDelta * TargetMatch.BarycentricWeight[Corner];
	//				}
	//				ensure(!MatchMorphDelta.PositionDelta.ContainsNaN());
	//				ensure(!MatchMorphDelta.TangentZDelta.ContainsNaN());
	//			}

	//			//Make sure all morph delta that are at the same position use the same delta to avoid hole in the geometry
	//			TArray<uint32>* MorphTargetsIndexUsingPosition = nullptr;
	//			MorphTargetsIndexUsingPosition = MorphTargetPerPosition.Find(SearchPosition);
	//			if (MorphTargetsIndexUsingPosition != nullptr)
	//			{
	//				//Get the maximum position/tangent delta for the existing matched morph delta
	//				FVector PositionDelta = MatchMorphDelta.PositionDelta;
	//				FVector TangentZDelta = MatchMorphDelta.TangentZDelta;
	//				for (uint32 ExistingMorphTargetIndex : *MorphTargetsIndexUsingPosition)
	//				{
	//					const FMorphTargetDelta& ExistingMorphDelta = OutDeltas[ExistingMorphTargetIndex];
	//					PositionDelta = PositionDelta.SizeSquared() > ExistingMorphDelta.PositionDelta.SizeSquared() ? PositionDelta : ExistingMorphDelta.PositionDelta;
	//					TangentZDelta = TangentZDelta.SizeSquared() > ExistingMorphDelta.TangentZDelta.SizeSquared() ? TangentZDelta : ExistingMorphDelta.TangentZDelta;
	//				}
	//				//Update all MorphTarget that share the same position.
	//				for (uint32 ExistingMorphTargetIndex : *MorphTargetsIndexUsingPosition)
	//				{
	//					FMorphTargetDelta& ExistingMorphDelta = OutDeltas[ExistingMorphTargetIndex];
	//					ExistingMorphDelta.PositionDelta = PositionDelta;
	//					ExistingMorphDelta.TangentZDelta = TangentZDelta;
	//				}
	//				MatchMorphDelta.PositionDelta = PositionDelta;
	//				MatchMorphDelta.TangentZDelta = TangentZDelta;
	//				MorphTargetsIndexUsingPosition->Add(OutDeltas.Num());
	//			}
	//			else
	//			{
	//				MorphTargetPerPosition.Add(TargetVertices[TargetIndex].Position).Add(OutDeltas.Num());
	//			}
	//			OutDeltas.Add(MatchMorphDelta);
	//		}
	//	}
	//	return OutDeltas.Num() > 0;
	//}
	return false;
}

bool UMeshEdittingLibrary::ApplyMorphTargetToLOD(USkeletalMesh* SkeletalMesh, const TArray<FMorphTargetDelta>& Deltas, int32 SourceLOD, int32 DestinationLOD, TArray<FMorphTargetDelta>& OutDeltas)
{
	//check(SkeletalMesh);
	//FSkeletalMeshModel* SkeletalMeshResource = SkeletalMesh->GetImportedModel();
	//if (!SkeletalMeshResource ||
	//	!SkeletalMeshResource->LODModels.IsValidIndex(SourceLOD) ||
	//	!SkeletalMeshResource->LODModels.IsValidIndex(DestinationLOD) ||
	//	SourceLOD > DestinationLOD)
	//{
	//	//Cannot reduce if the source model is missing or we reduce from a higher index LOD
	//	return false;
	//}

	//FSkeletalMeshLODModel& SourceLODModel = SkeletalMeshResource->LODModels[SourceLOD];

	//const FSkeletalMeshLODModel& BaseLODModel = SkeletalMeshResource->LODModels[SourceLOD];
	//const FSkeletalMeshLODModel& TargetLODModel = SkeletalMeshResource->LODModels[DestinationLOD];

	//UE::Geometry::FDynamicMesh3 BaseDynamicMesh;
	//UE::Geometry::FDynamicMesh3 TargetDynamicMesh;

	//if (!SkeletalMeshToDynamicMesh(SkeletalMesh, BaseDynamicMesh, NULL, TArray<FFinalSkinVertex>(), SourceLOD))
	//{
	//	return false;
	//}

	//if (!SkeletalMeshToDynamicMesh(SkeletalMesh, TargetDynamicMesh, NULL, TArray<FFinalSkinVertex>(), DestinationLOD))
	//{
	//	return false;
	//}


	//auto InternalGetSectionMaterialIndex = [](const FSkeletalMeshLODModel& LODModel, int32 SectionIndex)->int32
	//{
	//	if (!LODModel.Sections.IsValidIndex(SectionIndex))
	//	{
	//		return 0;
	//	}
	//	return LODModel.Sections[SectionIndex].MaterialIndex;
	//};

	//auto GetBaseSectionMaterialIndex = [&BaseLODModel, &InternalGetSectionMaterialIndex](int32 SectionIndex)->int32
	//{
	//	return InternalGetSectionMaterialIndex(BaseLODModel, SectionIndex);
	//};

	//auto GetTargetSectionMaterialIndex = [&TargetLODModel, &InternalGetSectionMaterialIndex](int32 SectionIndex)->int32
	//{
	//	return InternalGetSectionMaterialIndex(TargetLODModel, SectionIndex);
	//};

	////Make sure we have some morph for this LOD
	//if (Deltas.Num() <= 0)
	//{
	//	return false;
	//}

	////We have to match target sections index with the correct base section index. Reduced LODs can contain a different number of sections than the base LOD
	//TArray<int32> TargetSectionMatchBaseIndex;
	////Initialize the array to INDEX_NONE
	//TargetSectionMatchBaseIndex.AddUninitialized(TargetLODModel.Sections.Num());
	//for (int32 TargetSectionIndex = 0; TargetSectionIndex < TargetLODModel.Sections.Num(); ++TargetSectionIndex)
	//{
	//	TargetSectionMatchBaseIndex[TargetSectionIndex] = INDEX_NONE;
	//}
	//TBitArray<> BaseSectionMatch;
	//BaseSectionMatch.Init(false, BaseLODModel.Sections.Num());
	////Find corresponding section indices from Source LOD for Target LOD
	//for (int32 TargetSectionIndex = 0; TargetSectionIndex < TargetLODModel.Sections.Num(); ++TargetSectionIndex)
	//{
	//	int32 TargetSectionMaterialIndex = GetTargetSectionMaterialIndex(TargetSectionIndex);
	//	for (int32 BaseSectionIndex = 0; BaseSectionIndex < BaseLODModel.Sections.Num(); ++BaseSectionIndex)
	//	{
	//		if (BaseSectionMatch[BaseSectionIndex])
	//		{
	//			continue;
	//		}
	//		int32 BaseSectionMaterialIndex = GetBaseSectionMaterialIndex(BaseSectionIndex);
	//		if (TargetSectionMaterialIndex == BaseSectionMaterialIndex)
	//		{
	//			TargetSectionMatchBaseIndex[TargetSectionIndex] = BaseSectionIndex;
	//			BaseSectionMatch[BaseSectionIndex] = true;
	//			break;
	//		}
	//	}
	//}
	////We should have match all the target sections
	//check(!TargetSectionMatchBaseIndex.Contains(INDEX_NONE));
	//TArray<FSoftSkinVertex> BaseVertices;
	//TArray<FSoftSkinVertex> TargetVertices;
	//BaseLODModel.GetVertices(BaseVertices);
	//TargetLODModel.GetVertices(TargetVertices);
	////Create the base triangle indices per section
	//TArray<TArray<uint32>> BaseTriangleIndices;
	//int32 SectionCount = BaseLODModel.Sections.Num();
	//BaseTriangleIndices.AddDefaulted(SectionCount);

	//for (int32 SectionIndex = 0; SectionIndex < SectionCount; ++SectionIndex)
	//{
	//	const FSkelMeshSection& Section = BaseLODModel.Sections[SectionIndex];
	//	uint32 TriangleCount = Section.NumTriangles;
	//	BaseTriangleIndices[SectionIndex].SetNumZeroed(TriangleCount * 3);
	//}

	//for (int32 SectionIndex = 0; SectionIndex < SectionCount; ++SectionIndex)
	//{
	//	const FSkelMeshSection& Section = BaseLODModel.Sections[SectionIndex];
	//	uint32 TriangleCount = Section.NumTriangles;
	//	ParallelFor(TriangleCount, [&](int32 TriangleIndex)
	//		{
	//			for (uint32 PointIndex = 0; PointIndex < 3; PointIndex++)
	//			{
	//				const int32 Index = (TriangleIndex * 3) + PointIndex;
	//				uint32 IndexBufferValue = BaseLODModel.IndexBuffer[Section.BaseIndex + Index];
	//				BaseTriangleIndices[SectionIndex][Index] = IndexBufferValue;
	//			}
	//		});
	//}
	////Every target vertices match a Base LOD triangle, we also want the barycentric weight of the triangle match. All this done using the UVs
	//TArray<FMMTargetMatch> TargetMatchData;
	//TargetMatchData.AddUninitialized(TargetVertices.Num());
	////Match all target vertices to a Base triangle Using UVs.
	//ProjectTargetOnBase(BaseVertices, BaseTriangleIndices, TargetMatchData, TargetLODModel.Sections, TargetSectionMatchBaseIndex, *SkeletalMesh->GetName());
	////Helper to retrieve the FMorphTargetDelta from the BaseIndex
	//TMap<uint32, uint32> PerMorphTargetBaseIndexToMorphTargetDelta;
	////Create a map from BaseIndex to a list of match target index for all base morph target point
	//TMap<uint32, TArray<uint32>> BaseMorphIndexToTargetIndexList;

	//if (Deltas.Num() > 0)
	//{
	//	for (uint32 MorphDeltaIndex = 0; MorphDeltaIndex < (uint32)(Deltas.Num()); ++MorphDeltaIndex)
	//	{
	//		const FMorphTargetDelta& MorphDelta = Deltas[MorphDeltaIndex];
	//		PerMorphTargetBaseIndexToMorphTargetDelta.Add(MorphDelta.SourceIdx, MorphDeltaIndex);
	//		//Iterate the targetmatch data so we can store which target indexes is impacted by this morph delta.
	//		for (int32 TargetIndex = 0; TargetIndex < TargetMatchData.Num(); ++TargetIndex)
	//		{
	//			const FMMTargetMatch& TargetMatch = TargetMatchData[TargetIndex];
	//			if (TargetMatch.Indices[0] == INDEX_NONE)
	//			{
	//				//In case this vertex did not found a triangle match
	//				continue;
	//			}
	//			if (TargetMatch.Indices[0] == MorphDelta.SourceIdx || TargetMatch.Indices[1] == MorphDelta.SourceIdx || TargetMatch.Indices[2] == MorphDelta.SourceIdx)
	//			{
	//				TArray<uint32>& TargetIndexes = BaseMorphIndexToTargetIndexList.FindOrAdd(MorphDelta.SourceIdx);
	//				TargetIndexes.AddUnique(TargetIndex);
	//			}
	//		}
	//	}
	//	return CreateLODMorphTarget(Deltas, PerMorphTargetBaseIndexToMorphTargetDelta, BaseMorphIndexToTargetIndexList, TargetVertices, TargetMatchData, OutDeltas);
	//}
	return false;

}

///END of LODUtilities code

struct FCompareMorphTargetDeltas3
{
	FORCEINLINE bool operator()(const FMorphTargetDelta& A, const FMorphTargetDelta& B) const
	{
		return ((int32)A.SourceIdx - (int32)B.SourceIdx) < 0 ? true : false;
	}
};


bool UMeshEdittingLibrary::MergeMorphTargets(USkeletalMesh* SkeletalMesh, const TArray<FString>& MorphTargets, TArray<FMorphTargetDelta>& OutDeltas)
{
	/*if (SkeletalMesh)
	{
		SkeletalMesh->WaitForPendingInitOrStreaming();
		const FSkeletalMeshRenderData* Resource = SkeletalMesh->GetResourceForRendering();
		if (Resource)
		{
			if (Resource->LODRenderData.IsValidIndex(0))
			{
				const FSkeletalMeshLODRenderData& LODModel = Resource->LODRenderData[0];

				UE::Geometry::FDynamicMesh3 OriginalMesh;
				SkeletalMeshToDynamicMesh(SkeletalMesh, OriginalMesh);

				UE::Geometry::FDynamicMesh3 MeshCopy = OriginalMesh;


				GWarn->StatusForceUpdate(1, 3, FText::FromString("Retrieving Morph Target Deltas ..."));
				for (auto& MorphTarget : MorphTargets)
				{
					TArray<FMorphTargetDelta> Deltas;
					UMeshEdittingLibrary::GetMorphTargetDeltas(SkeletalMesh, MorphTarget, Deltas);
					if (Deltas.Num() > 0)
					{
						ApplyDeltasToDynamicMesh(Deltas, MeshCopy);
					}
				}

				GetMorphDeltas(OriginalMesh, MeshCopy, OutDeltas);

				return OutDeltas.Num() > 0;
			}
		}
	}*/
	return false;
}

bool UMeshEdittingLibrary::InvertMorphTarget(USkeletalMesh* SkeletalMesh, const FString& MorphTarget, TArray<FMorphTargetDelta>& OutDeltas)
{
	//if (SkeletalMesh)
	//{
	//	SkeletalMesh->WaitForPendingInitOrStreaming();
	//	const FSkeletalMeshRenderData* Resource = SkeletalMesh->GetResourceForRendering();
	//	if (Resource)
	//	{
	//		if (Resource->LODRenderData.IsValidIndex(0))
	//		{
	//			const FSkeletalMeshLODRenderData& LODModel = Resource->LODRenderData[0];

	//			UE::Geometry::FDynamicMesh3 OriginalMesh;
	//			SkeletalMeshToDynamicMesh(SkeletalMesh, OriginalMesh);

	//			UE::Geometry::FDynamicMesh3 MeshCopy = OriginalMesh;

	//			TArray<FMorphTargetDelta> Deltas;
	//			UMeshEdittingLibrary::GetMorphTargetDeltas(SkeletalMesh, MorphTarget, Deltas);

	//			if (Deltas.Num() > 0)
	//			{
	//				for (FMorphTargetDelta& Delta : Deltas)
	//				{
	//					Delta.PositionDelta *= FVector(-1.0f);
	//				}

	//				ApplyDeltasToDynamicMesh(Deltas, MeshCopy);

	//				GetMorphDeltas(OriginalMesh, MeshCopy, OutDeltas);
	//				return OutDeltas.Num() > 0;
	//			}
	//		}
	//	}
	//}
	return false;
}


void GetSmoothDeltas(const UE::Geometry::FDynamicMesh3& DynamicMesh, TArray<FMorphTargetDelta>& Deltas, const float& SmoothStrength, TArray<FMorphTargetDelta>& OutSmoothDeltas)
{
	/*const int32 TargetverticesNum = DynamicMesh.VertexCount();

	TArray<FVector> SmoothDeltas;
	TArray<TArray<FVector>> TempSmoothDeltas;
	SmoothDeltas.SetNumZeroed(TargetverticesNum);
	TempSmoothDeltas.SetNumZeroed(TargetverticesNum);

	ParallelFor(Deltas.Num(), [&](int32 Index)
		{
			const FMorphTargetDelta& NewDelta = Deltas[Index];
			SmoothDeltas[(int32)NewDelta.SourceIdx] = NewDelta.PositionDelta;
		});

	FCriticalSection Lock;

	ParallelFor(Deltas.Num(), [&](int32 Index)
		{
			const FMorphTargetDelta& NewDelta = Deltas[Index];
			for (const FIndex3i& Triangle : DynamicMesh.TrianglesItr())
			{
				if (NewDelta.SourceIdx == (uint32)Triangle[0] || NewDelta.SourceIdx == (uint32)Triangle[1] || NewDelta.SourceIdx == (uint32)Triangle[2])
				{
					Lock.Lock();
					TempSmoothDeltas[Triangle[0]].Add(((SmoothDeltas[Triangle[1]] + SmoothDeltas[Triangle[2]]) / 2 - SmoothDeltas[Triangle[0]]) * SmoothStrength + SmoothDeltas[Triangle[0]]);
					TempSmoothDeltas[Triangle[1]].Add(((SmoothDeltas[Triangle[2]] + SmoothDeltas[Triangle[0]]) / 2 - SmoothDeltas[Triangle[1]]) * SmoothStrength + SmoothDeltas[Triangle[1]]);
					TempSmoothDeltas[Triangle[2]].Add(((SmoothDeltas[Triangle[0]] + SmoothDeltas[Triangle[1]]) / 2 - SmoothDeltas[Triangle[2]]) * SmoothStrength + SmoothDeltas[Triangle[2]]);
					Lock.Unlock();
				}
			}
		});

	OutSmoothDeltas.Empty();

	ParallelFor(TempSmoothDeltas.Num(), [&](int32 Index)
		{
			if (TempSmoothDeltas[Index].Num() > 0)
			{
				FMorphTargetDelta NewDelta;
				NewDelta.SourceIdx = (uint32)Index;
				NewDelta.PositionDelta = FVector::ZeroVector;

				for (const FVector& Temp : TempSmoothDeltas[Index])
				{
					NewDelta.PositionDelta += Temp;
				}
				NewDelta.PositionDelta /= TempSmoothDeltas[Index].Num();
				Lock.Lock();
				OutSmoothDeltas.Add(NewDelta);
				Lock.Unlock();
			}
		});*/

}

void ApplyDeltasToIdenticalVertices(const TArray<TSet<int32>>& VerticesSets, TArray<FMorphTargetDelta>& Deltas)
{
	/*ParallelFor(VerticesSets.Num(), [&](int32 Index)
		{
			const auto& VerticeSet = VerticesSets[Index];

			FVector Sum = FVector::ZeroVector;

			for (const FMorphTargetDelta& NewDelta : Deltas)
			{
				if (VerticeSet.Contains((int32)NewDelta.SourceIdx))
				{
					Sum += NewDelta.PositionDelta;
				}
			}

			const FVector Average = Sum / VerticeSet.Num();

			for (FMorphTargetDelta& NewDelta : Deltas)
			{
				if (VerticeSet.Contains((int32)NewDelta.SourceIdx))
				{
					NewDelta.PositionDelta = Average;
				}
			}
		});*/
}


void CreateDeltasForVertexPairs(const TMap<int32, int32>& VertexPairs, const TArray<FMorphTargetDelta>& BaseDeltas, TArray<FMorphTargetDelta>& OutDeltas)
{
	/*TArray<FMorphTargetDelta> TempOutDeltas;
	for (const TPair<int32, int32>& VertexPair : VertexPairs)
	{

		FMorphTargetDelta NewDelta;
		NewDelta.PositionDelta = FVector::ZeroVector;
		NewDelta.SourceIdx = (uint32)VertexPair.Key;

		bool bHasValidVertex = false;
		FCriticalSection Lock;
		ParallelFor(BaseDeltas.Num(), [&](int32 Index)
			{
				if ((int32)BaseDeltas[Index].SourceIdx == VertexPair.Value)
				{
					Lock.Lock();
					NewDelta.PositionDelta += BaseDeltas[Index].PositionDelta;
					bHasValidVertex = true;
					Lock.Unlock();
				}
			});

		if (bHasValidVertex)
		{
			TempOutDeltas.Add(NewDelta);
		}
	}

	OutDeltas = MoveTemp(TempOutDeltas);*/
}

bool IsDynamicMeshIdentical(const UE::Geometry::FDynamicMesh3& DynamicMeshA, const UE::Geometry::FDynamicMesh3& DynamicMeshB)
{
	/*const int32 BaseverticesNumA = DynamicMeshA.VertexCount();
	const int32 BaseverticesNumB = DynamicMeshB.VertexCount();

	if (BaseverticesNumA == BaseverticesNumB)
	{
		for (int32 Index = 0; Index < BaseverticesNumB; Index++)
		{
			if (!FVector(DynamicMeshA.GetVertex(Index)).Equals(FVector(DynamicMeshB.GetVertex(Index))))
			{
				return false;
			}
		}
		return true;
	}*/
	return false;
}

bool IsDynamicMeshTrianglesIdentical(const UE::Geometry::FDynamicMesh3& DynamicMeshA, const UE::Geometry::FDynamicMesh3& DynamicMeshB)
{

	//const int32 BaseverticesNumA = DynamicMeshA.VertexCount();
	//const int32 BaseverticesNumB = DynamicMeshB.VertexCount();

	//const int32 BaseTrianglesNumA = DynamicMeshA.TriangleCount();
	//const int32 BaseTrianglesNumB = DynamicMeshB.TriangleCount();

	//if (BaseverticesNumA == BaseverticesNumB && BaseTrianglesNumA == BaseTrianglesNumB)
	//{
	//	for (int32 Index = 0; Index < BaseTrianglesNumB; Index++)
	//	{
	//		const FIndex3i TriangleA = DynamicMeshA.GetTriangle(Index);
	//		const FIndex3i TriangleB = DynamicMeshB.GetTriangle(Index);
	//		if (!TriangleA.Contains(TriangleB.A) || !TriangleA.Contains(TriangleB.B) || !TriangleA.Contains(TriangleB.C))
	//		{
	//			return false;
	//		}
	//	}
	//	return true;
	//}
	return false;
}

void CalculateDynamicMeshesForTransfer(const UE::Geometry::FDynamicMesh3& BaseDynamicMesh, const UE::Geometry::FDynamicMesh3& TargetDynamicMesh, const float& Threshold, TArray<TSet<int32>>& VerticesSets, TMap<int32, int32>& VertexPairs)
{
	/*VerticesSets.Empty();
	VertexPairs.Empty();

	const int32 BaseverticesNum = BaseDynamicMesh.VertexCount();
	const int32 TargetverticesNum = TargetDynamicMesh.VertexCount();

	FCriticalSection Lock;

	ParallelFor(TargetverticesNum, [&](int32 TargetIndex)
		{
			for (int32 NextTargetIndex = TargetIndex; NextTargetIndex < TargetverticesNum; NextTargetIndex++)
			{
				if (TargetDynamicMesh.IsVertex(TargetIndex) && TargetDynamicMesh.IsVertex(NextTargetIndex))
				{
					if (FVector(TargetDynamicMesh.GetVertex(TargetIndex)).Equals(FVector(TargetDynamicMesh.GetVertex(NextTargetIndex))))
					{
						bool bHasSet = false;
						Lock.Lock();
						for (TSet<int32>& VerticesSet : VerticesSets)
						{
							if (VerticesSet.Contains(TargetIndex) || VerticesSet.Contains(NextTargetIndex))
							{
								VerticesSet.Add(TargetIndex);
								VerticesSet.Add(NextTargetIndex);
								bHasSet = true;
								break;
							}
						}
						if (!bHasSet)
						{
							VerticesSets.Add({ TargetIndex, NextTargetIndex });
						}
						Lock.Unlock();
					}
				}
			}

			bool bHasValidVertex = false;
			float LastDistance = Threshold * 3;
			int32 ClosestIndex = 0;
			for (int32 BaseIndex = 0; BaseIndex < BaseverticesNum; BaseIndex++)
			{
				if (BaseDynamicMesh.IsVertex(BaseIndex) && TargetDynamicMesh.IsVertex(TargetIndex))
				{
					const float Distance = (FVector(BaseDynamicMesh.GetVertex(BaseIndex)) - FVector(TargetDynamicMesh.GetVertex(TargetIndex))).Size();
					if (Distance < Threshold && Distance < LastDistance)
					{
						LastDistance = Distance;
						ClosestIndex = BaseIndex;
						bHasValidVertex = true;
					}
				}
			}

			if (bHasValidVertex)
			{
				Lock.Lock();
				VertexPairs.Add(TargetIndex, ClosestIndex);
				Lock.Unlock();
			}

		});*/
}

bool UMeshEdittingLibrary::CopyMorphTarget(USkeletalMesh* SkeletalMesh, const TArray<FString>& MorphTargets, USkeletalMesh* TargetSkeletalMesh, TArray<TArray<FMorphTargetDelta>>& OutDeltas, float Threshold, float Multiplier, int32 SmoothIterations, float SmoothStrength)
{
	/*if (SkeletalMesh && TargetSkeletalMesh)
	{
		SkeletalMesh->WaitForPendingInitOrStreaming();
		const FSkeletalMeshRenderData* Resource = SkeletalMesh->GetResourceForRendering();
		if (Resource)
		{
			if (Resource->LODRenderData.IsValidIndex(0))
			{
				const FSkeletalMeshLODRenderData& LODModel = Resource->LODRenderData[0];

				GWarn->StatusForceUpdate(1, 3, FText::FromString("Converting Skeletal Mesh to Dynamic Mesh ..."));
				UE::Geometry::FDynamicMesh3 BaseDynamicMesh;
				if (!SkeletalMeshToDynamicMesh(SkeletalMesh, BaseDynamicMesh))
				{
					return false;
				}

				UE::Geometry::FDynamicMesh3 OriginalDynamicMesh;
				if (!SkeletalMeshToDynamicMesh(TargetSkeletalMesh, OriginalDynamicMesh))
				{
					return false;
				}

				const int32 BaseverticesNum = BaseDynamicMesh.VertexCount();

				TArray<TSet<int32>> VerticesSets;
				TMap<int32, int32> VertexPairs;

				const bool bIdentical = IsDynamicMeshIdentical(BaseDynamicMesh, OriginalDynamicMesh);
				bool SameTopology = false;

				if (!bIdentical)
				{
					SameTopology = IsDynamicMeshTrianglesIdentical(BaseDynamicMesh, OriginalDynamicMesh);
					if (!SameTopology)
					{
						CalculateDynamicMeshesForTransfer(BaseDynamicMesh, OriginalDynamicMesh, Threshold, VerticesSets, VertexPairs);
					}
				}

				GWarn->StatusForceUpdate(2, 3, FText::FromString("Retrieving Morph Target Deltas ..."));
				for (const FString& MorphTarget : MorphTargets)
				{
					TArray<FMorphTargetDelta>& Deltas = OutDeltas.AddDefaulted_GetRef();
					GetMorphTargetDeltas(SkeletalMesh, MorphTarget, Deltas);
					if (bIdentical || SameTopology)
					{
						continue;
					}
					else {
						UE::Geometry::FDynamicMesh3 TargetDynamicMesh = OriginalDynamicMesh;
						const int32 TargetverticesNum = TargetDynamicMesh.VertexCount();

						CreateDeltasForVertexPairs(VertexPairs, Deltas, Deltas);

						if (SmoothIterations > 0)
						{
							for (int32 SmoothIndex = 0; SmoothIndex < SmoothIterations; SmoothIndex++)
							{
								GetSmoothDeltas(TargetDynamicMesh, Deltas, SmoothStrength, Deltas);
							}
						}


						ApplyDeltasToIdenticalVertices(VerticesSets, Deltas);

						ParallelFor(Deltas.Num(), [&](int32 Index)
							{
								const FMorphTargetDelta& Delta = Deltas[Index];
								const FVector3d Position = TargetDynamicMesh.GetVertex(Delta.SourceIdx) + (FVector3d(Delta.PositionDelta) * Multiplier);
								TargetDynamicMesh.SetVertex(Delta.SourceIdx, Position);
							});

						GetMorphDeltas(OriginalDynamicMesh, TargetDynamicMesh, Deltas);
					}
				}

				return true;
			}
		}
	}*/
	return false;
}

void UMeshEdittingLibrary::ApplySourceDeltasToDynamicMesh(const UE::Geometry::FDynamicMesh3& SourceDynamicMesh, const UE::Geometry::FDynamicMesh3& DynamicMesh, const TArray<FMorphTargetDelta>& SourceDeltas, TArray<FMorphTargetDelta>& OutDeltas, float Threshold, float Multiplier, int32 SmoothIterations, float SmoothStrength, bool bCheckIdentical)
{
	/*OutDeltas.Empty();

	UE::Geometry::FDynamicMesh3 TargetDynamicMesh = DynamicMesh;

	const int32 BaseverticesNum = SourceDynamicMesh.VertexCount();

	TArray<TSet<int32>> VerticesSets;
	TMap<int32, int32> VertexPairs;

	const bool bIdentical = bCheckIdentical ? IsDynamicMeshIdentical(SourceDynamicMesh, TargetDynamicMesh) : false;

	if (!bIdentical)
	{
		CalculateDynamicMeshesForTransfer(SourceDynamicMesh, TargetDynamicMesh, Threshold, VerticesSets, VertexPairs);
	}

	const int32 TargetverticesNum = TargetDynamicMesh.VertexCount();


	if (bIdentical)
	{
		OutDeltas = SourceDeltas;
	}
	else {
		TArray<FMorphTargetDelta> NewMorphTargetDeltas;
		CreateDeltasForVertexPairs(VertexPairs, SourceDeltas, NewMorphTargetDeltas);

		if (SmoothIterations > 0)
		{
			for (int32 SmoothIndex = 0; SmoothIndex < SmoothIterations; SmoothIndex++)
			{
				GetSmoothDeltas(TargetDynamicMesh, NewMorphTargetDeltas, SmoothStrength, NewMorphTargetDeltas);
			}
		}

		ApplyDeltasToIdenticalVertices(VerticesSets, NewMorphTargetDeltas);

		ParallelFor(NewMorphTargetDeltas.Num(), [&](int32 Index)
			{
				const FMorphTargetDelta& Delta = NewMorphTargetDeltas[Index];
				const FVector3d Position = TargetDynamicMesh.GetVertex(Delta.SourceIdx) + (FVector3d(Delta.PositionDelta) * Multiplier);
				TargetDynamicMesh.SetVertex(Delta.SourceIdx, Position);
			});

		GetMorphDeltas(DynamicMesh, TargetDynamicMesh, NewMorphTargetDeltas);
		OutDeltas = NewMorphTargetDeltas;
	}*/
}

bool UMeshEdittingLibrary::ImportMorphTarget(USkeletalMesh* SkeletalMesh, const TArray<UBAMorphTarget*>& MorphTargets, USkeletalMesh* TargetSkeletalMesh, TArray<TArray<FMorphTargetDelta>>& OutDeltas, float Threshold, float Multiplier, int32 SmoothIterations, float SmoothStrength)
{
	/*if (SkeletalMesh && TargetSkeletalMesh)
	{
		SkeletalMesh->WaitForPendingInitOrStreaming();
		const FSkeletalMeshRenderData* Resource = SkeletalMesh->GetResourceForRendering();
		if (Resource)
		{
			if (Resource->LODRenderData.IsValidIndex(0))
			{
				const FSkeletalMeshLODRenderData& LODModel = Resource->LODRenderData[0];

				GWarn->StatusForceUpdate(1, 3, FText::FromString("Converting Skeletal Mesh to Dynamic Mesh ..."));
				UE::Geometry::FDynamicMesh3 BaseDynamicMesh;
				if (!SkeletalMeshToDynamicMesh(SkeletalMesh, BaseDynamicMesh))
				{
					return false;
				}

				UE::Geometry::FDynamicMesh3 OriginalDynamicMesh;
				if (!SkeletalMeshToDynamicMesh(TargetSkeletalMesh, OriginalDynamicMesh))
				{
					return false;
				}

				const int32 BaseverticesNum = BaseDynamicMesh.VertexCount();

				TArray<TSet<int32>> VerticesSets;
				TMap<int32, int32> VertexPairs;

				const bool bIdentical = IsDynamicMeshIdentical(BaseDynamicMesh, OriginalDynamicMesh);
				bool SameTopology = false;

				if (!bIdentical)
				{
					SameTopology = IsDynamicMeshTrianglesIdentical(BaseDynamicMesh, OriginalDynamicMesh);
					if (!SameTopology)
					{
						CalculateDynamicMeshesForTransfer(BaseDynamicMesh, OriginalDynamicMesh, Threshold, VerticesSets, VertexPairs);
					}
				}

				GWarn->StatusForceUpdate(2, 3, FText::FromString("Retrieving Morph Target Deltas ..."));
				for (UBAMorphTarget* MorphTarget : MorphTargets)
				{
					TArray<FMorphTargetDelta>& Deltas = OutDeltas.AddDefaulted_GetRef();
					if (MorphTarget && MorphTarget->HasDataForLOD(0))
					{
						Deltas = MorphTarget->MorphLODModels[0].Vertices;
						if (bIdentical || SameTopology)
						{
							continue;
						}
						else {
							UE::Geometry::FDynamicMesh3 TargetDynamicMesh = OriginalDynamicMesh;
							const int32 TargetverticesNum = TargetDynamicMesh.VertexCount();

							CreateDeltasForVertexPairs(VertexPairs, Deltas, Deltas);

							if (SmoothIterations > 0)
							{
								for (int32 SmoothIndex = 0; SmoothIndex < SmoothIterations; SmoothIndex++)
								{
									GetSmoothDeltas(TargetDynamicMesh, Deltas, SmoothStrength, Deltas);
								}
							}


							ApplyDeltasToIdenticalVertices(VerticesSets, Deltas);

							ParallelFor(Deltas.Num(), [&](int32 Index)
								{
									const FMorphTargetDelta& Delta = Deltas[Index];
									const FVector3d Position = TargetDynamicMesh.GetVertex(Delta.SourceIdx) + (FVector3d(Delta.PositionDelta) * Multiplier);
									TargetDynamicMesh.SetVertex(Delta.SourceIdx, Position);
								});

							GetMorphDeltas(OriginalDynamicMesh, TargetDynamicMesh, Deltas);
						}
					}
				}

				return true;
			}
		}
	}*/
	return false;
}

bool UMeshEdittingLibrary::CreateMorphTargetFromPose(USkeletalMesh* SkeletalMesh, const UE::Geometry::FDynamicMesh3& PoseDynamicMesh, TArray<FMorphTargetDelta>& OutDeltas)
{
	/*if (SkeletalMesh)
	{
		OutDeltas.Empty();
		SkeletalMesh->WaitForPendingInitOrStreaming();
		const FSkeletalMeshRenderData* Resource = SkeletalMesh->GetResourceForRendering();
		if (Resource)
		{
			if (Resource->LODRenderData.IsValidIndex(0))
			{
				const FSkeletalMeshLODRenderData& LODModel = Resource->LODRenderData[0];

				UE::Geometry::FDynamicMesh3 BaseDynamicMesh;
				GWarn->StatusForceUpdate(1, 3, FText::FromString("Converting Skeletal Mesh to Dynamic Mesh ..."));
				if (!SkeletalMeshToDynamicMesh(SkeletalMesh, BaseDynamicMesh))
				{
					return false;
				}
				GWarn->StatusForceUpdate(2, 3, FText::FromString("Retrieving Morph Target Deltas ..."));
				GetMorphDeltas(BaseDynamicMesh, PoseDynamicMesh, OutDeltas);
				return OutDeltas.Num() > 0;
			}
		}
	}*/

	return false;
}

bool UMeshEdittingLibrary::CreateMorphTargetFromMesh(USkeletalMesh* SkeletalMesh, USkeletalMesh* SourceSkeletalMesh, TArray<FMorphTargetDelta>& OutDeltas, float Threshold, float Multiplier, int32 SmoothIterations, float SmoothStrength)
{
	/*if (SkeletalMesh && SourceSkeletalMesh)
	{
		OutDeltas.Empty();
		SkeletalMesh->WaitForPendingInitOrStreaming();
		SourceSkeletalMesh->WaitForPendingInitOrStreaming();
		const FSkeletalMeshRenderData* Resource = SkeletalMesh->GetResourceForRendering();
		const FSkeletalMeshRenderData* SourceResource = SourceSkeletalMesh->GetResourceForRendering();
		if (Resource && SourceResource)
		{
			UE::Geometry::FDynamicMesh3 BaseDynamicMesh;
			UE::Geometry::FDynamicMesh3 SourceDynamicMesh;
			int32 FoundLOD = INDEX_NONE;
			GWarn->StatusForceUpdate(1, 3, FText::FromString("Finding Matching LODs ..."));
			for (int32 LOD = 0; LOD < Resource->LODRenderData.Num(); ++LOD)
			{
				if (Resource->LODRenderData.IsValidIndex(LOD) && SourceResource->LODRenderData.IsValidIndex(LOD))
				{
					if (!SkeletalMeshToDynamicMesh(SkeletalMesh, BaseDynamicMesh, NULL, TArray<FFinalSkinVertex>(), LOD))
					{
						return false;
					}

					if (!SkeletalMeshToDynamicMesh(SourceSkeletalMesh, SourceDynamicMesh, NULL, TArray<FFinalSkinVertex>(), LOD))
					{
						return false;
					}

					if (IsDynamicMeshTrianglesIdentical(BaseDynamicMesh, SourceDynamicMesh))
					{
						FoundLOD = LOD;
						break;
					}

				}
			}
			GWarn->StatusForceUpdate(2, 3, FText::FromString("Retrieving Morph Target Deltas ..."));
			if (FoundLOD != INDEX_NONE)
			{
				if (FoundLOD > 0)
				{
					TArray<FMorphTargetDelta> Deltas;
					GetMorphDeltas(BaseDynamicMesh, SourceDynamicMesh, Deltas);

					UE::Geometry::FDynamicMesh3 BaseDynamicMeshLOD0;
					if (!SkeletalMeshToDynamicMesh(SkeletalMesh, BaseDynamicMeshLOD0))
					{
						return false;
					}

					ApplySourceDeltasToDynamicMesh(BaseDynamicMesh, BaseDynamicMeshLOD0, Deltas, OutDeltas, Threshold, Multiplier, SmoothIterations, SmoothStrength, true);

					return OutDeltas.Num() > 0;

				}
				else {
					GetMorphDeltas(BaseDynamicMesh, SourceDynamicMesh, OutDeltas);
					return OutDeltas.Num() > 0;
				}

			}
		}
	}*/
	return false;
}

void SubdivideMesh(UE::Geometry::FDynamicMesh3& Mesh)
{
	/*TArray<int> EdgesToProcess;
	for (int tid : Mesh.EdgeIndicesItr())
	{
		EdgesToProcess.Add(tid);
	}
	int MaxTriangleID = Mesh.MaxTriangleID();

	TArray<int> TriSplitEdges;
	TriSplitEdges.Init(-1, Mesh.MaxTriangleID());

	for (int eid : EdgesToProcess)
	{
		FIndex2i EdgeTris = Mesh.GetEdgeT(eid);

		UE::Geometry::FDynamicMesh3::FEdgeSplitInfo SplitInfo;
		EMeshResult result = Mesh.SplitEdge(eid, SplitInfo);
		if (result == EMeshResult::Ok)
		{
			if (EdgeTris.A < MaxTriangleID && TriSplitEdges[EdgeTris.A] == -1)
			{
				TriSplitEdges[EdgeTris.A] = SplitInfo.NewEdges.B;
			}
			if (EdgeTris.B != UE::Geometry::FDynamicMesh3::InvalidID)
			{
				if (EdgeTris.B < MaxTriangleID && TriSplitEdges[EdgeTris.B] == -1)
				{
					TriSplitEdges[EdgeTris.B] = SplitInfo.NewEdges.C;
				}
			}
		}
	}

	for (int eid : TriSplitEdges)
	{
		if (eid != -1)
		{
			UE::Geometry::FDynamicMesh3::FEdgeFlipInfo FlipInfo;
			Mesh.FlipEdge(eid, FlipInfo);
		}
	}*/
}


void UMeshEdittingLibrary::ReadObj(const FString& Path, UE::Geometry::FDynamicMesh3& Mesh, bool bYUp)
{

	//Mesh.Clear();

	//FString ObjData;
	//const bool bBaseResult = FFileHelper::LoadFileToString(ObjData, *Path);

	//TArray<FString> OutArray;
	//ObjData.ParseIntoArrayLines(OutArray);

	//TArray<FIndex3i> Triangles;

	//for (FString& Line : OutArray)
	//{
	//	if (Line[0] == 'v')
	//	{
	//		if (Line[1] == ' ')
	//		{
	//			FVector3d Position;
	//			sscanf_s(TCHAR_TO_ANSI(*Line), "v %lf %lf %lf", &Position.X, &Position.Y, &Position.Z);
	//			FTransform3d Transform;
	//			Transform.SetRotation(FQuaterniond(FVector3d(0, 0, 1), 180, true));
	//			Position = Transform.TransformPosition(Position);
	//			if (bYUp)
	//			{
	//				Transform.SetRotation(FQuaterniond(FVector3d(1, 0, 0), 90, true));
	//				Position = Transform.InverseTransformPosition(Position);
	//			}
	//			Mesh.AppendVertex(Position);
	//		}
	//	}
	//	else if (Line[0] == 'f')
	//	{
	//		FIndex3i Triangle;
	//		if (!Line.Contains("/"))
	//		{
	//			sscanf_s(TCHAR_TO_ANSI(*Line), "f %d %d %d", &Triangle.A, &Triangle.B, &Triangle.C);
	//			for (int32 i = 0; i < 3; ++i)
	//			{
	//				Triangle[i] -= 1;
	//			}
	//			Triangles.Add(Triangle);
	//		}
	//		else {
	//			//This is a mess. We don't really need triangles but we could fix this just in case.
	//			TArray<FString> TriangleArray;
	//			Line.ParseIntoArrayWS(TriangleArray);
	//			if (TriangleArray.Num() >= 4)
	//			{
	//				TriangleArray.RemoveAt(0);

	//				for (int32 i = 0; i < 3; ++i)
	//				{
	//					TriangleArray[i].ReplaceCharInline('//', '/');
	//					TArray<FString> TriString;
	//					TriangleArray[i].ParseIntoArray(TriString, TEXT("/"));
	//					Triangle[i] = FCString::Atoi(*TriString[0]) - 1;
	//				}
	//				Triangles.Add(Triangle);
	//			}
	//		}
	//	}
	//}

	//for (const FIndex3i& Triangle : Triangles)
	//{
	//	Mesh.AppendTriangle(Triangle);
	//}

	//ParallelFor(Mesh.VertexCount(), [&](int32 Index)
	//	{
	//		if (Mesh.IsVertex(Index))
	//		{
	//			Mesh.SetVertexNormal(Index, FVector3f(FMeshNormals::ComputeVertexNormal(Mesh, Index)));
	//		}
	//	});

}

bool UMeshEdittingLibrary::CreateMorphTargetFromObj(USkeletalMesh* SkeletalMesh, const FFilePath& BaseObjPath, const FFilePath& MorphedObjPath, TArray<FMorphTargetDelta>& OutDeltas, float Threshold, float Multiplier, int32 SmoothIterations, float SmoothStrength, bool bYUp)
{

	//if (SkeletalMesh)
	//{
	//	SkeletalMesh->WaitForPendingInitOrStreaming();
	//	const FSkeletalMeshRenderData* Resource = SkeletalMesh->GetResourceForRendering();
	//	if (Resource)
	//	{
	//		if (Resource->LODRenderData.IsValidIndex(0))
	//		{
	//			const FSkeletalMeshLODRenderData& LODModel = Resource->LODRenderData[0];
	//			UE::Geometry::FDynamicMesh3 DynamicMesh;

	//			GWarn->StatusForceUpdate(1, 7, FText::FromString("Converting Skeletal Mesh to Dynamic Mesh ..."));
	//			if (!SkeletalMeshToDynamicMesh(SkeletalMesh, DynamicMesh))
	//			{
	//				return false;
	//			}

	//			UE::Geometry::FDynamicMesh3 BaseMesh;
	//			UE::Geometry::FDynamicMesh3 MorphedMesh;
	//			GWarn->StatusForceUpdate(2, 7, FText::FromString("Converting Base Mesh OBJ to Dynamic Mesh ..."));
	//			ReadObj(BaseObjPath.FilePath, BaseMesh, bYUp);
	//			GWarn->StatusForceUpdate(3, 7, FText::FromString("Converting Morphed Mesh OBJ to Dynamic Mesh ..."));
	//			ReadObj(MorphedObjPath.FilePath, MorphedMesh, bYUp);

	//			//UE_LOG(LogTemp, Warning, TEXT("Base. %d\n"), BaseMesh.VertexCount());
	//			//UE_LOG(LogTemp, Warning, TEXT("Morph. %d\n"), MorphedMesh.VertexCount());

	//			GWarn->StatusForceUpdate(4, 7, FText::FromString("Checking Base Mesh OBJ has same Topology with Morphed Mesh OBJ ..."));
	//			if (IsDynamicMeshTrianglesIdentical(BaseMesh, MorphedMesh) && BaseMesh.VertexCount() > 0)
	//			{
	//				TArray<FMorphTargetDelta> InitialDeltas;
	//				GWarn->StatusForceUpdate(5, 7, FText::FromString("Retrieving Morph Target Deltas ..."));
	//				GetMorphDeltas(BaseMesh, MorphedMesh, InitialDeltas);

	//				//UE_LOG(LogTemp, Warning, TEXT("Deltas Found. %d\n"), InitialDeltas.Num());


	//				OutDeltas.Empty();
	//				GWarn->StatusForceUpdate(6, 7, FText::FromString("Aplying Morph Target Deltas to Dynamic Mesh ..."));
	//				ApplySourceDeltasToDynamicMesh(BaseMesh, DynamicMesh, InitialDeltas, OutDeltas, Threshold, Multiplier, SmoothIterations, SmoothStrength, true);

	//				return OutDeltas.Num() > 0;
	//			}
	//		}
	//	}
	//}

	return false;
}

#undef LOCTEXT_NAMESPACE