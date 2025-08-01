// Created by Jorge Puerto. All Rights Reserved.

#include "DlgMergeSkeleton.h"

#include "Widgets/SWindow.h"
#include "Widgets/SBoxPanel.h"
#include "Styling/SlateTypes.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"

#define LOCTEXT_NAMESPACE "BADlgMergeSkeleton"

//BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SDlgMergeSkeleton::Construct(const FArguments& InArgs)
{
	UserResponse = FDlgMergeSkeleton::RM_Cancel;
	ParentWindow = InArgs._ParentWindow.Get();

	this->ChildSlot[
		SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.0f, 4.0f, 8.0f, 4.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("MergeSkeletonDlgDescription", "Would you like to add following bones to the skeleton?"))
			]
		+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.0f, 4.0f, 8.0f, 4.0f)
			[
				SNew(SSeparator)
			]
		+ SVerticalBox::Slot()
			.Padding(8.0f, 4.0f, 8.0f, 4.0f)
			[
				SNew(SBorder)
				[
					SNew(SScrollBox)
					+ SScrollBox::Slot()
			[
				//Save this widget so we can populate it later with check boxes
				SAssignNew(CheckBoxContainer, SVerticalBox)
			]
				]
			]
		+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.Padding(8.0f, 4.0f, 8.0f, 4.0f)
			[
				SNew(SUniformGridPanel)
				.SlotPadding(FAppStyle::GetMargin("StandardDialog.SlotPadding"))
			.MinDesiredSlotWidth(FAppStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
			.MinDesiredSlotHeight(FAppStyle::GetFloat("StandardDialog.MinDesiredSlotHeight"))
			+ SUniformGridPanel::Slot(0, 0)
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
			.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
			.OnClicked(this, &SDlgMergeSkeleton::ChangeAllOptions, true)
			.Text(LOCTEXT("SkeletonMergeSelectAll", "Select All"))
			]
		+ SUniformGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
			.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
			.OnClicked(this, &SDlgMergeSkeleton::ChangeAllOptions, false)
			.Text(LOCTEXT("SkeletonMergeDeselectAll", "Deselect All"))
			]
			]
		+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.0f, 4.0f, 8.0f, 4.0f)
			[
				SNew(SSeparator)
			]
		+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Right)
			.Padding(8.0f, 4.0f, 8.0f, 4.0f)
			[
				SNew(SUniformGridPanel)
				.SlotPadding(FAppStyle::GetMargin("StandardDialog.SlotPadding"))
			.MinDesiredSlotWidth(FAppStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
			.MinDesiredSlotHeight(FAppStyle::GetFloat("StandardDialog.MinDesiredSlotHeight"))
			+ SUniformGridPanel::Slot(0, 0)
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
			.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
			.OnClicked(this, &SDlgMergeSkeleton::OnButtonClick, FDlgMergeSkeleton::RM_Confirm)
			.Text(LOCTEXT("SkeletonMergeOk", "OK"))
			]
		+ SUniformGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
			.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
			.OnClicked(this, &SDlgMergeSkeleton::OnButtonClick, FDlgMergeSkeleton::RM_Cancel)
			.Text(LOCTEXT("SkeletonMergeCancel", "Cancel"))
			]
			]
	];
}
//END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<SWidget> SDlgMergeSkeleton::CreateCheckBox(const FString& Label, int32 ButtonId)
{
	return
		SNew(SCheckBox)
		.IsChecked(this, &SDlgMergeSkeleton::IsCheckboxChecked, ButtonId)
		.OnCheckStateChanged(this, &SDlgMergeSkeleton::OnCheckboxChanged, ButtonId)
		[
			SNew(STextBlock).Text(FText::FromString(Label))
		];
}

ECheckBoxState SDlgMergeSkeleton::IsCheckboxChecked(int32 ButtonId) const
{
	return CheckBoxInfoMap.FindChecked(ButtonId).bUsed ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SDlgMergeSkeleton::OnCheckboxChanged(ECheckBoxState NewCheckboxState, int32 CheckboxThatChanged)
{
	FBoneCheckboxInfo& Info = CheckBoxInfoMap.FindChecked(CheckboxThatChanged);
	Info.bUsed = !Info.bUsed;
}

FReply SDlgMergeSkeleton::ChangeAllOptions(bool bNewCheckedState)
{
	for (auto Iter = CheckBoxInfoMap.CreateIterator(); Iter; ++Iter)
	{
		FBoneCheckboxInfo& Info = Iter.Value();
		Info.bUsed = bNewCheckedState;
	}
	return FReply::Handled();
}


void SDlgMergeSkeleton::PopulateOptions(TArray<FBoneCheckboxInfo>& BoneInfos)
{
	for (auto Iter = BoneInfos.CreateIterator(); Iter; ++Iter)
	{
		FBoneCheckboxInfo& Info = (*Iter);
		Info.bUsed = true;

		CheckBoxInfoMap.Add(Info.BoneID, Info);

		CheckBoxContainer->AddSlot()
			.AutoHeight()
			[
				CreateCheckBox(Info.BoneName.GetPlainNameString(), Info.BoneID)
			];
	}
}


FDlgMergeSkeleton::EResult_Merge SDlgMergeSkeleton::GetUserResponse() const
{
	return UserResponse;
}


bool SDlgMergeSkeleton::IsBoneIncluded(int32 BoneID)
{
	auto* Item = CheckBoxInfoMap.Find(BoneID);
	return Item ? Item->bUsed : false;
}


FReply SDlgMergeSkeleton::OnButtonClick(FDlgMergeSkeleton::EResult_Merge ButtonID)
{
	ParentWindow->RequestDestroyWindow();
	UserResponse = ButtonID;

	return FReply::Handled();
}



///////////////////////////////////////////////////////////////////////////////////////

FDlgMergeSkeleton::FDlgMergeSkeleton(USkeletalMesh* InMesh, USkeleton* InSkeleton)
{
	Mesh = InMesh;
	Skeleton = InSkeleton;

	if (FSlateApplication::IsInitialized())
	{
		DialogWindow = SNew(SWindow)
			.Title(LOCTEXT("MergeSkeletonDlgTitle", "Merge Bones"))
			.SupportsMinimize(false).SupportsMaximize(false)
			.ClientSize(FVector2D(350, 500));

		TSharedPtr<SBorder> DialogWrapper =
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			.Padding(4.0f)
			[
				SAssignNew(DialogWidget, SDlgMergeSkeleton)
				.ParentWindow(DialogWindow)
			];

		DialogWindow->SetContent(DialogWrapper.ToSharedRef());
	}
}

FDlgMergeSkeleton::EResult_Merge FDlgMergeSkeleton::ShowModal()
{
	RequiredBones.Empty();

	TMap<FName, int32> BoneIndicesMap;
	TArray<FBoneCheckboxInfo> BoneInfos;

	// Make a list of all skeleton bone list
	const FReferenceSkeleton& RefSkeleton = Skeleton->GetReferenceSkeleton();
	for (int32 BoneTreeId = 0; BoneTreeId < RefSkeleton.GetRawBoneNum(); ++BoneTreeId)
	{
		const FName& BoneName = RefSkeleton.GetBoneName(BoneTreeId);
		BoneIndicesMap.Add(BoneName, BoneTreeId);
	}

	for (int32 RefBoneId = 0; RefBoneId < Mesh->GetRefSkeleton().GetRawBoneNum(); ++RefBoneId)
	{
		const FName& BoneName = Mesh->GetRefSkeleton().GetBoneName(RefBoneId);
		// if I can't find this from Skeleton
		if (BoneIndicesMap.Find(BoneName) == NULL)
		{
			FBoneCheckboxInfo Info;
			Info.BoneID = RefBoneId;
			Info.BoneName = BoneName;
			BoneInfos.Add(Info);
		}
	}

	if (BoneInfos.Num() == 0)
	{
		// it's all identical, but still need to return RequiredBones
		// for the case, where they'd like to replace the one exactly same hierarchy but different skeleton 
		for (int32 RefBoneId = 0; RefBoneId < Mesh->GetRefSkeleton().GetRawBoneNum(); ++RefBoneId)
		{
			const FName& BoneName = Mesh->GetRefSkeleton().GetBoneName(RefBoneId);
			RequiredBones.Add(RefBoneId);
		}

		return EResult_Merge::RM_Confirm;
	}

	DialogWidget->PopulateOptions(BoneInfos);

	//Show Dialog
	GEditor->EditorAddModalWindow(DialogWindow.ToSharedRef());
	EResult_Merge UserResponse = (EResult_Merge)DialogWidget->GetUserResponse();

	if (UserResponse == EResult_Merge::RM_Confirm)
	{
		for (int32 RefBoneId = 0; RefBoneId < Mesh->GetRefSkeleton().GetRawBoneNum(); ++RefBoneId)
		{
			if (DialogWidget->IsBoneIncluded(RefBoneId))
			{
				TArray<int32> ParentList;

				// I need to make sure parent exists first
				int32 ParentIndex = Mesh->GetRefSkeleton().GetParentIndex(RefBoneId);

				// make sure RequiredBones already have ParentIndex
				while (ParentIndex >= 0)
				{
					// if I don't have it yet
					if (RequiredBones.Contains(ParentIndex) == false)
					{
						ParentList.Add(ParentIndex);
					}

					ParentIndex = Mesh->GetRefSkeleton().GetParentIndex(ParentIndex);
				}

				if (ParentList.Num() > 0)
				{
					// if we need to add parent list
					// add from back to front (since it's added from child to up
					for (int32 I = ParentList.Num() - 1; I >= 0; --I)
					{
						RequiredBones.Add(ParentList[I]);
					}
				}

				RequiredBones.Add(RefBoneId);
			}
		}
	}
	return UserResponse;
}

#undef LOCTEXT_NAMESPACE