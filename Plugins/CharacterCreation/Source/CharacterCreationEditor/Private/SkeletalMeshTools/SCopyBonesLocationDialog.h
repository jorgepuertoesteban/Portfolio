// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SReferencePoseOptionsDialog.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWindow.h"


enum ECopyPoseType
{
	ECPT_Cancel,
	ECPT_CurrentPose,
	ECPT_SelectedAsset
};


class SCopyBonesLocationDialog : public SWindow
{
public:
	SLATE_BEGIN_ARGS(SCopyBonesLocationDialog)
		{
		}

		SLATE_ARGUMENT(UDebugSkelMeshComponent*, CurrentMesh)
	SLATE_END_ARGS()

	SCopyBonesLocationDialog()
		: UserResponse(ECopyPoseType::ECPT_Cancel)
	{
	}

	void Construct(const FArguments& InArgs);


	/** Gets the resulting full asset path (path+'/'+name) */
	TWeakObjectPtr<UObject> GetSelectedAsset() const
	{
		return PickedMeshOrSkeleton;
	}

	FRefUpdatePoseOptions ShowModal();

protected:
	FReply OnButtonClick(ECopyPoseType ButtonID);


	bool AllowCancelClick;

	ECopyPoseType UserResponse;

	TWeakObjectPtr<UDebugSkelMeshComponent> EdittingMeshComp;

	bool OnlyAllowCompatibleSkeletons(const FAssetData& InAssetData) const;
	void OnSourceMeshOrSkeletonPicked(const FAssetData& InSelectedAssetData);

	TWeakObjectPtr<UObject> PickedMeshOrSkeleton;

private:
	FRefUpdatePoseOptions SelectedOptions;

	ECheckBoxState IsUpdateRefSkelChecked() const
	{
		return SelectedOptions.bUpdateReferenceSkeleton ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	void OnUpdateRefSkelChange(const ECheckBoxState NewState)
	{
		SelectedOptions.bUpdateReferenceSkeleton = NewState == ECheckBoxState::Checked ? true : false;
		if (SelectedOptions.bUpdateReferenceSkeleton && SelectedOptions.bUpdateMesh)
		{
			SelectedOptions.bUpdateMesh = false;
		}
		else if (!SelectedOptions.bUpdateReferenceSkeleton && !SelectedOptions.bUpdateMesh)
		{
			// its radio, at least one must be selected
			SelectedOptions.bUpdateReferenceSkeleton = true;
		}
	}

	ECheckBoxState IsUpdateMeshGeoChecked() const
	{
		return SelectedOptions.bUpdateMesh ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	void OnUpdateMeshChange(const ECheckBoxState NewState)
	{
		SelectedOptions.bUpdateMesh = NewState == ECheckBoxState::Checked ? true : false;
		if (SelectedOptions.bUpdateReferenceSkeleton && SelectedOptions.bUpdateMesh)
		{
			SelectedOptions.bUpdateReferenceSkeleton = false;
		}
		else if (!SelectedOptions.bUpdateReferenceSkeleton && !SelectedOptions.bUpdateMesh)
		{
			// its radio, at least one must be selected
			SelectedOptions.bUpdateMesh = true;
		}
	}

	//rotation
	ECheckBoxState IsRotationsChecked() const
	{
		return SelectedOptions.bRotations ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	void OnRotationCheckStateChange(const ECheckBoxState NewState)
	{
		SelectedOptions.bRotations = NewState == ECheckBoxState::Checked ? true : false;
	}

	//scale
	ECheckBoxState IsScaleChecked() const
	{
		return SelectedOptions.bScale ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	void OnScaleCheckStateChange(const ECheckBoxState NewState)
	{
		SelectedOptions.bScale = NewState == ECheckBoxState::Checked ? true : false;
	}

	//translation
	ECheckBoxState IsTranslationChecked() const
	{
		return SelectedOptions.bLocations ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	void OnTranslationCheckStateChange(const ECheckBoxState NewState)
	{
		SelectedOptions.bLocations = NewState == ECheckBoxState::Checked ? true : false;
	}
};
