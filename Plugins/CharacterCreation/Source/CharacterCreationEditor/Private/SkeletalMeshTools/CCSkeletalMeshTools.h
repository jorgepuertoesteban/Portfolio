// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUndoClient.h"

class FUICommandList;
class FExtender;
class ISkeletalMeshEditor;

class FCCSkeletalMeshTools : public FEditorUndoClient
{

protected:

	TSharedPtr<FExtender> MenuExtender;
	//TSharedPtr<FExtender> ToolbarExtender;
	TSharedPtr<FUICommandList> Actions;
	// Handle for the extender delegate we created
	FDelegateHandle SkelMeshEditorExtenderHandle;

public:

	void Register();

	void Unregister();

	static void ExtendSkelMeshEditorToolbar(const TSharedRef<FUICommandList> InCommandList, TSharedRef<ISkeletalMeshEditor> InSkeletalMeshEditor);

	static void AddSubMenu(FMenuBuilder& MenuBuilder);

	static void AddMenuEntry(FMenuBuilder& MenuBuilder);

	static void OpenMeshEditor(const TWeakPtr<ISkeletalMeshEditor>& Ed);

	static void OpenPoseMatcher(TWeakPtr<ISkeletalMeshEditor> Ed);

	static void CopyBonesLocation(TWeakPtr<ISkeletalMeshEditor> Ed);

	static void ResetModifiedBones(TWeakPtr<ISkeletalMeshEditor> Ed);

	static void DeleteExtraBones(TWeakPtr<ISkeletalMeshEditor> Ed);
	/*
	static void CopyBonesToPreview(TWeakPtr<ISkeletalMeshEditor> Ed);
	static void SaveCurrentReferencePose(TWeakPtr<ISkeletalMeshEditor> Ed);
	*/
	static void BakeCurrentGeometry(TWeakPtr<ISkeletalMeshEditor> Ed);

	static void ForceAssignSkeleton(TWeakPtr<ISkeletalMeshEditor> Ed);
	
	//FEditorUndoClient
	void PostUndo(bool bSuccess) override;
	void PostRedo(bool bSuccess) override;
	//End FEditorUndoClient
};
