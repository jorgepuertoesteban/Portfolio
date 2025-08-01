// Created by Jorge Puerto. All Rights Reserved.

	
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
* FBoneCheckboxInfo
*
* Context data for the SDlgMergeSkeleton panel check boxes
*/
struct FBoneCheckboxInfo
{
	FName BoneName;
	int32 BoneID;
	bool  bUsed;
};

/**
* FDlgMergeSkeleton
*
* Wrapper class for SDlgMergeSkeleton. This class creates and launches a dialog then awaits the
* result to return to the user.
*/
class FDlgMergeSkeleton
{
public:
	enum EResult_Merge
	{
		RM_Cancel = 0,			// No/Cancel, normal usage would stop the current action
		RM_Confirm = 1,		// Yes/Ok/Etc, normal usage would continue with action
	};

	FDlgMergeSkeleton(USkeletalMesh* InMesh, USkeleton* InSkeleton);

	/**  Shows the dialog box and waits for the user to respond. */
	EResult_Merge ShowModal();

	// List of required bones for skeleton
	TArray<int32> RequiredBones;
private:

	/** Cached pointer to the modal window */
	TSharedPtr<SWindow> DialogWindow;

	/** Cached pointer to the merge skeleton widget */
	TSharedPtr<class SDlgMergeSkeleton> DialogWidget;

	/** The SkeletalMesh to merge bones from*/
	USkeletalMesh* Mesh;
	/** The Skeleton to merge bones to */
	USkeleton* Skeleton;
};

/**
 * Slate panel for choosing which bones to merge into the skeleton
 */
class SDlgMergeSkeleton : public SCompoundWidget
{


public:

	SLATE_BEGIN_ARGS(SDlgMergeSkeleton) {}
	/** Window in which this widget resides */
		SLATE_ATTRIBUTE(TSharedPtr<SWindow>, ParentWindow)
	SLATE_END_ARGS()


private:
	/** Stores the users response to this dialog */
	FDlgMergeSkeleton::EResult_Merge	 UserResponse;

	/** The slate container that the bone check boxes get added to */
	TSharedPtr<SVerticalBox>	 CheckBoxContainer;
	/** Store the check box state for each bone */
	TMap<int32, FBoneCheckboxInfo> CheckBoxInfoMap;

	/** Pointer to the window which holds this Widget, required for modal control */
	TSharedPtr<SWindow>			 ParentWindow;

public:

	void Construct(const FArguments& InArgs);


	/**
	* Creates a Slate check box
	*
	* @param	Label		Text label for the check box
	* @param	ButtonId	The ID for the check box
	* @return				The created check box widget
	*/
	TSharedRef<SWidget> CreateCheckBox(const FString& Label, int32 ButtonId);

	/**
	 * Returns the state of the check box
	 *
	 * @param	ButtonId	The ID for the check box
	 * @return				The status of the check box
	 */
	ECheckBoxState IsCheckboxChecked(int32 ButtonId) const;

	/**
	 * Handler for all check box clicks
	 *
	 * @param	NewCheckboxState	The new state of the check box
	 * @param	CheckboxThatChanged	The ID of the radio button that has changed.
	 */
	void OnCheckboxChanged(ECheckBoxState NewCheckboxState, int32 CheckboxThatChanged);

	/**
	 * Handler for the Select All and Deselect All buttons
	 *
	 * @param	bNewCheckedState	The new state of the check boxes
	 */
	FReply ChangeAllOptions(bool bNewCheckedState);

	/**
	 * Populated the dialog with multiple check boxes, each corresponding to a bone
	 *
	 * @param	BoneInfos	The list of Bones to populate the dialog with
	 */
	void PopulateOptions(TArray<FBoneCheckboxInfo>& BoneInfos);

	/**
	 * Returns the EResult of the button which the user pressed. Closing of the dialog
	 * in any other way than clicking "Ok" results in this returning a "Cancel" value
	 */
	FDlgMergeSkeleton::EResult_Merge GetUserResponse() const;

	/**
	 * Returns whether the user selected that bone to be used (checked its respective check box)
	 */
	bool IsBoneIncluded(int32 BoneID);

private:

	/**
	 * Handles when a button is pressed, should be bound with appropriate EResult Key
	 *
	 * @param ButtonID - The return type of the button which has been pressed.
	 */
	FReply OnButtonClick(FDlgMergeSkeleton::EResult_Merge ButtonID);
};