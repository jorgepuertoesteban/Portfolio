// Created by Jorge Puerto. All Rights Reserved.

#include "SkeletalMeshTools/SCopyBonesLocationDialog.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Misc/PackageName.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Editor.h"
#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "SReferencePoseOptionsDialog.h"
#include "Animation/DebugSkelMeshComponent.h"

#define LOCTEXT_NAMESPACE "CopyBonesLocationDialog"

void SCopyBonesLocationDialog::Construct(const FArguments& InArgs)
{
	AllowCancelClick = true;
	SelectedOptions = FRefUpdatePoseOptions();
	EdittingMeshComp = TWeakObjectPtr<UDebugSkelMeshComponent>(InArgs._CurrentMesh);

	FAssetPickerConfig AssetPickerConfig;
	AssetPickerConfig.Filter.ClassPaths.Add(USkeleton::StaticClass()->GetClassPathName());
	AssetPickerConfig.Filter.bRecursiveClasses = true;
	AssetPickerConfig.SelectionMode = ESelectionMode::Single;
	AssetPickerConfig.bAllowNullSelection = true;
	AssetPickerConfig.InitialAssetViewType = EAssetViewType::List;
	AssetPickerConfig.bCanShowRealTimeThumbnails = false;
	AssetPickerConfig.ThumbnailScale = 0.25f;
	AssetPickerConfig.OnAssetSelected.BindRaw(this, &SCopyBonesLocationDialog::OnSourceMeshOrSkeletonPicked);
	// only allow compatible skeletons to be selected
	//AssetPickerConfig.OnShouldFilterAsset = FOnShouldFilterAsset::CreateLambda(
	//	[](const FAssetData& InAssetData) -> bool
	//	{
	//		return !(InAssetData.GetAsset() && InAssetData.GetAsset()->IsA(USkeleton::StaticClass()));
	//	}
	//);

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	SWindow::Construct(SWindow::FArguments()
		.Title(LOCTEXT("ReferencePoseOptionsTitle", "Options"))
		.SupportsMinimize(false)
		.SupportsMaximize(false)
		.ClientSize(FVector2D(900, 500))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot() // Add user input block
		.Padding(2)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()		[
			SNew(STextBlock)
			.Text(LOCTEXT("SelectPath", "Select a Skeletal Mesh or Skeleton for source pose (or none for current pose)"))
		.Font(FCoreStyle::GetDefaultFontStyle("Regular", 14))
		]
	+ SVerticalBox::Slot()
		.MaxHeight(450)
		.Padding(3)
		[
			ContentBrowserModule.Get().CreateAssetPicker(AssetPickerConfig)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SSeparator)
		]
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		.Padding(5)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.FillWidth(1)
		[
			SNew(SCheckBox)
			.IsChecked(this, &SCopyBonesLocationDialog::IsRotationsChecked)
		.IsEnabled_Lambda([this]() { return SelectedOptions.bUpdateReferenceSkeleton; })
		.ToolTipText(LOCTEXT("UpdateRotationsCheckboxTooltip", "Updates the bone rotations when updating the reference skeleton"))
		.OnCheckStateChanged(this, &SCopyBonesLocationDialog::OnRotationCheckStateChange)
		.Content()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("UpdateRefSkeletonRotation", "Rotations"))
		]
		]
	+ SHorizontalBox::Slot()
		.FillWidth(1)
		[
			SNew(SCheckBox)
			.IsChecked(this, &SCopyBonesLocationDialog::IsTranslationChecked)
		.IsEnabled_Lambda([this]() { return SelectedOptions.bUpdateReferenceSkeleton; })
		.ToolTipText(LOCTEXT("UpdateLocationCheckboxTooltip", "Updates the bone translations when updating the reference skeleton"))
		.OnCheckStateChanged(this, &SCopyBonesLocationDialog::OnTranslationCheckStateChange)
		.Content()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("UpdateRefSkeletonLocation", "Location"))
		]
		]
	+ SHorizontalBox::Slot()
		.FillWidth(1)
		[
			SNew(SCheckBox)
			.IsChecked(this, &SCopyBonesLocationDialog::IsScaleChecked)
		.IsEnabled_Lambda([this]() { return SelectedOptions.bUpdateReferenceSkeleton; })
		.ToolTipText(LOCTEXT("UpdateScaleCheckboxTooltip", "Updates the bone scales when updating the reference skeleton"))
		.OnCheckStateChanged(this, &SCopyBonesLocationDialog::OnScaleCheckStateChange)
		.Content()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("UpdateRefSkeletonScale", "Scale"))
		]
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		.Padding(5)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.FillWidth(1)
		[
			SNew(SCheckBox)
			.Style(&FAppStyle::GetWidgetStyle<FCheckBoxStyle>("Menu.RadioButton"))
		.IsChecked(this, &SCopyBonesLocationDialog::IsUpdateRefSkelChecked)
		.ToolTipText(LOCTEXT("UpdateRefSkeletonCheckboxTooltip", "Updates the reference skeleton bones. This is generally what you want."))
		.OnCheckStateChanged(this, &SCopyBonesLocationDialog::OnUpdateRefSkelChange)
		.Content()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("UpdateRefSkeleton", "Update Ref Skeleton?"))
		]
		]
	+ SHorizontalBox::Slot()
		.FillWidth(1)
		[
			SNew(SCheckBox)
			.Style(&FAppStyle::GetWidgetStyle<FCheckBoxStyle>("Menu.RadioButton"))
		.IsChecked(this, &SCopyBonesLocationDialog::IsUpdateMeshGeoChecked)
		.ToolTipText(LOCTEXT("UpdateMeshGeoCheckboxTooltip", "Updates the mesh vertex positions. (Experimental)"))
		.OnCheckStateChanged(this, &SCopyBonesLocationDialog::OnUpdateMeshChange)
		.Content()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("UpdateMeshGeo", "Update Mesh Geometry?"))
		]
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Right)
		.Padding(5)
		[
			SNew(SUniformGridPanel)
			.SlotPadding(FAppStyle::GetMargin("StandardDialog.SlotPadding"))
		.MinDesiredSlotWidth(FAppStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
		.MinDesiredSlotHeight(FAppStyle::GetFloat("StandardDialog.MinDesiredSlotHeight"))
		+ SUniformGridPanel::Slot(0, 0)
		.HAlign(HAlign_Left)
		[
			SNew(SButton)
			.HAlign(HAlign_Left)
		.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
		.Text(LOCTEXT("Cancel", "Cancel"))
		.OnClicked(this, &SCopyBonesLocationDialog::OnButtonClick, ECopyPoseType::ECPT_Cancel)
		.IsEnabled(true)
		]
			+ SUniformGridPanel::Slot(1, 0)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
		.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
		.Text(LOCTEXT("UseCurrentPose", "Use Current Pose"))
		.IsEnabled_Lambda([this]()
			{
				if (SelectedOptions.bUpdateReferenceSkeleton)
				{
					return SelectedOptions.bLocations || SelectedOptions.bScale || SelectedOptions.bRotations;
				}
				return SelectedOptions.bUpdateMesh || SelectedOptions.bUpdateReferenceSkeleton;
			})
		.ToolTipText(LOCTEXT("UseCurrentPoseTooltip", "Use the pose of the preview mesh as the pose source"))
				.OnClicked(this, &SCopyBonesLocationDialog::OnButtonClick, ECopyPoseType::ECPT_CurrentPose)
		]
	+ SUniformGridPanel::Slot(2, 0)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
		.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
		.Text(LOCTEXT("UseSelectedPose", "Use Selected asset pose"))
		.ToolTipText(LOCTEXT("UseSelectedPoseTooltip", "Use the selected mesh or skeleton as the pose source"))
		.IsEnabled_Lambda([this]()
			{
				if (SelectedOptions.bUpdateReferenceSkeleton)
				{
					return SelectedOptions.bLocations || SelectedOptions.bScale || SelectedOptions.bRotations;
				}
				return PickedMeshOrSkeleton != nullptr && (
					SelectedOptions.bUpdateMesh || SelectedOptions.bUpdateReferenceSkeleton
					);
			})
		.OnClicked(this, &SCopyBonesLocationDialog::OnButtonClick, ECopyPoseType::ECPT_SelectedAsset)
		]

		]
		]
	);
}

FReply SCopyBonesLocationDialog::OnButtonClick(ECopyPoseType ButtonID)
{
	SelectedOptions.UpdateType = (ERefPoseOptionsReturnType::Type)ButtonID;
	RequestDestroyWindow();
	return FReply::Handled();
}

FRefUpdatePoseOptions SCopyBonesLocationDialog::ShowModal()
{
	GEditor->EditorAddModalWindow(SharedThis(this));
	return SelectedOptions;
}

bool SCopyBonesLocationDialog::OnlyAllowCompatibleSkeletons(const FAssetData& InAssetData) const
{
	return true;
}

void SCopyBonesLocationDialog::OnSourceMeshOrSkeletonPicked(const FAssetData& InSelectedAssetData)
{
	PickedMeshOrSkeleton = TWeakObjectPtr<UObject>(InSelectedAssetData.GetAsset());
}

#undef LOCTEXT_NAMESPACE

