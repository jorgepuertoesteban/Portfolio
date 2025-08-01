// Created by Jorge Puerto. All Rights Reserved.

#include "CharacterCreationEditAction_GenerateCharacterCreatorGroom.h"

#include "EngineUtils.h"

#include "UObject/NoExportTypes.h"

#include "Kismet2/SClassPickerDialog.h"
#include "Editor/EditorEngine.h"
#include "Editor.h"

#include "GenerateCharacterCreatorGroomCommand.h"

#include "Engine/SkeletalMesh.h"
#include "GroomAsset.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/AppStyle.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SSeparator.h"

#define LOCTEXT_NAMESPACE "CharacterCreationEditAction_GenerateCharacterCreatorGroom"



class SGenerateCharacterCreatorGroomWindow : public SWindow
{
public:
	void Construct(const FArguments& InArgs)
	{

		// Load the content browser module to display an asset picker
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

		FAssetPickerConfig GroomPickerConfig;
		GroomPickerConfig.SelectionMode = ESelectionMode::Single;
		/** The asset picker will only show sequences */
		GroomPickerConfig.Filter.ClassPaths.Add(UGroomAsset::StaticClass()->GetClassPathName());
		/** The delegate that fires when an asset was selected */
		GroomPickerConfig.OnAssetSelected = FOnAssetSelected::CreateRaw(this, &SGenerateCharacterCreatorGroomWindow::OnGroomAssetSelected);

		/** The default view mode should be a list view */
		GroomPickerConfig.InitialAssetViewType = EAssetViewType::Tile;
		GroomPickerConfig.InitialThumbnailSize = EThumbnailSize::Small;

		FAssetPickerConfig SourceSkeletalMeshPickerConfig;
		SourceSkeletalMeshPickerConfig.SelectionMode = ESelectionMode::Single;
		/** The asset picker will only show sequences */
		SourceSkeletalMeshPickerConfig.Filter.ClassPaths.Add(USkeletalMesh::StaticClass()->GetClassPathName());
		/** The delegate that fires when an asset was selected */
		SourceSkeletalMeshPickerConfig.OnAssetSelected = FOnAssetSelected::CreateRaw(this, &SGenerateCharacterCreatorGroomWindow::OnSourceSkeletalMeshSelected);

		/** The default view mode should be a list view */
		SourceSkeletalMeshPickerConfig.InitialAssetViewType = EAssetViewType::Tile;
		SourceSkeletalMeshPickerConfig.InitialThumbnailSize = EThumbnailSize::Small;

		FAssetPickerConfig TargetSkeletalMeshesPickerConfig;
		TargetSkeletalMeshesPickerConfig.SelectionMode = ESelectionMode::Multi;
		/** The asset picker will only show sequences */
		TargetSkeletalMeshesPickerConfig.Filter.ClassPaths.Add(USkeletalMesh::StaticClass()->GetClassPathName());
		/** The default view mode should be a list view */
		TargetSkeletalMeshesPickerConfig.InitialAssetViewType = EAssetViewType::Tile;
		TargetSkeletalMeshesPickerConfig.InitialThumbnailSize = EThumbnailSize::Small;
		TargetSkeletalMeshesPickerConfig.GetCurrentSelectionDelegates.Add(&GetCurrentSelectionDelegate);
		TargetSkeletalMeshesPickerConfig.OnAssetSelected = FOnAssetSelected::CreateRaw(this, &SGenerateCharacterCreatorGroomWindow::OnTargetSkeletalMeshesSelected);

		SWindow::Construct(SWindow::FArguments()
			.Title(LOCTEXT("CreateGroomBinding", "Create Groom Binding"))
			.SizingRule(ESizingRule::UserSized)
			.ClientSize(FVector2D(1500, 1500*16/9))
			.SupportsMinimize(false)
			.SupportsMaximize(false)
			[
				SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("Menu.Background"))
					[
						SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.FillHeight(0.4)
							.Padding(3, 3)
							[
								SNew(SVerticalBox)

									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(2, 2)
									[
										SNew(STextBlock)
											.Text(FText::FromString(TEXT("Select Groom Asset ")))
									]

									+ SVerticalBox::Slot()
									.Padding(2, 2)
									[
										ContentBrowserModule.Get().CreateAssetPicker(GroomPickerConfig)
									]
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SSeparator)
									.Orientation(Orient_Horizontal)
							]
							+ SVerticalBox::Slot()
							.FillHeight(0.4)
							.Padding(3, 3)
							[
								SNew(SVerticalBox)

									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(2, 2)
									[
										SNew(STextBlock)
											.Text(FText::FromString(TEXT("Select Source Skeletal Mesh ")))
									]

									+ SVerticalBox::Slot()
									.Padding(2, 2)
									[
										ContentBrowserModule.Get().CreateAssetPicker(SourceSkeletalMeshPickerConfig)
									]
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SSeparator)
									.Orientation(Orient_Horizontal)
							]
							+ SVerticalBox::Slot()
							.FillHeight(0.4)
							.Padding(3, 3)
							[
								SNew(SVerticalBox)

									+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(2, 2)
									[
										SNew(STextBlock)
											.Text(FText::FromString(TEXT("Select Target Skeletal Mesh ")))
									]

									+ SVerticalBox::Slot()
									.Padding(2, 2)
									[
										ContentBrowserModule.Get().CreateAssetPicker(TargetSkeletalMeshesPickerConfig)
									]
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SSeparator)
									.Orientation(Orient_Horizontal)
							]

							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SUniformGridPanel)
									.SlotPadding(FAppStyle::GetMargin("StandardDialog.SlotPadding"))
									.MinDesiredSlotWidth(FAppStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
									.MinDesiredSlotHeight(FAppStyle::GetFloat("StandardDialog.MinDesiredSlotHeight"))
									+ SUniformGridPanel::Slot(0, 0)
									[
										SNew(SButton)
											.Text(LOCTEXT("Accept", "Accept"))
											.HAlign(HAlign_Center)
											.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
											.IsEnabled(this, &SGenerateCharacterCreatorGroomWindow::CanAccept)
											.OnClicked_Raw(this, &SGenerateCharacterCreatorGroomWindow::OnAccept)
									]
									+ SUniformGridPanel::Slot(1, 0)
									[
										SNew(SButton)
											.Text(LOCTEXT("Cancel", "Cancel"))
											.HAlign(HAlign_Center)
											.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
											.OnClicked_Raw(this, &SGenerateCharacterCreatorGroomWindow::OnCancel)
									]

							]
					]
			]);
	}

	bool CanAccept() const
	{
		return GenerateCharacterCreatorGroomOptions.GroomAsset != nullptr  && GenerateCharacterCreatorGroomOptions.SourceSkeletalMesh != nullptr && !GenerateCharacterCreatorGroomOptions.TargetSkeletalMeshes.IsEmpty() ;
	}

	FReply OnAccept()
	{

		GenerateCharacterCreatorGroomCommand::Run(GenerateCharacterCreatorGroomOptions);

		RequestDestroyWindow();
		return FReply::Handled();
	}

	FReply OnCancel()
	{
		RequestDestroyWindow();
		return FReply::Handled();
	}

	void OnGroomAssetSelected(const FAssetData& SelectedAsset)
	{
		GenerateCharacterCreatorGroomOptions.GroomAsset = Cast<UGroomAsset>(SelectedAsset.GetAsset());
	}

	void OnSourceSkeletalMeshSelected(const FAssetData& SelectedAsset)
	{
		GenerateCharacterCreatorGroomOptions.SourceSkeletalMesh = Cast<USkeletalMesh>(SelectedAsset.GetAsset());
	}

	void OnTargetSkeletalMeshesSelected(const FAssetData& SelectedAsset)
	{
		GenerateCharacterCreatorGroomOptions.TargetSkeletalMeshes.Empty();
		TArray<FAssetData> SelectedAssets = GetCurrentSelectionDelegate.Execute();
		for (const FAssetData& Asset : SelectedAssets)
		{
			GenerateCharacterCreatorGroomOptions.TargetSkeletalMeshes.Add(Cast<USkeletalMesh>(Asset.GetAsset()));
		}
	}
	
private:

	FGenerateCharacterCreatorGroomOptions GenerateCharacterCreatorGroomOptions;
	FGetCurrentSelectionDelegate GetCurrentSelectionDelegate;

};


void FCharacterCreationEditAction_GenerateCharacterCreatorGroom::ExecuteAction()
{
	TSharedPtr<SWindow> PickerWindow = SNew(SGenerateCharacterCreatorGroomWindow);
	GEditor->EditorAddModalWindow(PickerWindow.ToSharedRef());
	PickerWindow.Reset();
}


#undef LOCTEXT_NAMESPACE