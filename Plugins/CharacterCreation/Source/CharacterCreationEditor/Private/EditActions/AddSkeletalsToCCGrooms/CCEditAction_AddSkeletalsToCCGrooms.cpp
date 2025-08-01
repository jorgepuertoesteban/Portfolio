// Created by Jorge Puerto. All Rights Reserved.

#include "EditActions/AddSkeletalsToCCGrooms/CCEditAction_AddSkeletalsToCCGrooms.h"

#include "Editor/EditorEngine.h"
#include "Editor.h"

#include "AddSkeletalsToCCGroomsCommand.h"
#include "CharacterCreatorGroom.h"

#include "Engine/SkeletalMesh.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/AppStyle.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SSeparator.h"

#define LOCTEXT_NAMESPACE "CCEditAction_AddSkeletalsToCCGrooms"


class SAddSkeletalsToCharacterCreationGroomsWindow : public SWindow
{
public:
	void Construct(const FArguments& InArgs)
	{
		// Load the content browser module to display an asset picker
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

		FAssetPickerConfig GroomsPickerConfig;
		GroomsPickerConfig.SelectionMode = ESelectionMode::Multi;
		GroomsPickerConfig.InitialAssetViewType = EAssetViewType::Tile;
		GroomsPickerConfig.InitialThumbnailSize = EThumbnailSize::Small;
		GroomsPickerConfig.Filter.ClassPaths.Add(UCharacterCreatorGroom::StaticClass()->GetClassPathName());
		GroomsPickerConfig.GetCurrentSelectionDelegates.Add(&GroomsCurrentSelectionDelegate);
		GroomsPickerConfig.OnAssetSelected = FOnAssetSelected::CreateRaw(this, &SAddSkeletalsToCharacterCreationGroomsWindow::OnGroomsSelected);


		FAssetPickerConfig MeshesPickerConfig;
		MeshesPickerConfig.SelectionMode = ESelectionMode::Multi;
		MeshesPickerConfig.InitialAssetViewType = EAssetViewType::Tile;
		MeshesPickerConfig.InitialThumbnailSize = EThumbnailSize::Small;
		MeshesPickerConfig.Filter.ClassPaths.Add(USkeletalMesh::StaticClass()->GetClassPathName());
		MeshesPickerConfig.GetCurrentSelectionDelegates.Add(&MeshesCurrentSelectionDelegate);
		MeshesPickerConfig.OnAssetSelected = FOnAssetSelected::CreateRaw(this, &SAddSkeletalsToCharacterCreationGroomsWindow::OnMeshesSelected);


		FAssetPickerConfig SourceMeshPickerConfig;
		SourceMeshPickerConfig.SelectionMode = ESelectionMode::Single;
		SourceMeshPickerConfig.InitialAssetViewType = EAssetViewType::Tile;
		SourceMeshPickerConfig.InitialThumbnailSize = EThumbnailSize::Small;
		SourceMeshPickerConfig.Filter.ClassPaths.Add(USkeletalMesh::StaticClass()->GetClassPathName());
		SourceMeshPickerConfig.OnAssetSelected = FOnAssetSelected::CreateRaw(this, &SAddSkeletalsToCharacterCreationGroomsWindow::OnSourceMeshSelected);

		SWindow::Construct(SWindow::FArguments()
							.Title(LOCTEXT("CreateGroomBinding", "Create Groom Binding"))
							.SizingRule(ESizingRule::UserSized)
							.ClientSize(FVector2D(1500, 1500 * 16 / 9))
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
							.Text(FText::FromString(TEXT("Select Groom Assets")))
						]

						+ SVerticalBox::Slot()
						.Padding(2, 2)
						[
							ContentBrowserModule.Get().CreateAssetPicker(GroomsPickerConfig)
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
							.Text(FText::FromString(TEXT("Select New Skeletal Meshes ")))
						]

						+ SVerticalBox::Slot()
						.Padding(2, 2)
						[
							ContentBrowserModule.Get().CreateAssetPicker(MeshesPickerConfig)
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
							ContentBrowserModule.Get().CreateAssetPicker(SourceMeshPickerConfig)
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
							.IsEnabled(this, &SAddSkeletalsToCharacterCreationGroomsWindow::CanAccept)
							.OnClicked_Raw(this, &SAddSkeletalsToCharacterCreationGroomsWindow::OnAccept)
						]
						+ SUniformGridPanel::Slot(1, 0)
						[
							SNew(SButton)
							.Text(LOCTEXT("Cancel", "Cancel"))
							.HAlign(HAlign_Center)
							.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
							.OnClicked_Raw(this, &SAddSkeletalsToCharacterCreationGroomsWindow::OnCancel)
						]

					]
				]
			]);
	}

	bool CanAccept() const
	{
		return !AddSkeletalsToCharacterCreationGroomsOptions.Grooms.IsEmpty() && !AddSkeletalsToCharacterCreationGroomsOptions.Meshes.IsEmpty();
	}

	FReply OnAccept()
	{
		AddSkeletalsToCCGroomsCommand::Run(AddSkeletalsToCharacterCreationGroomsOptions);

		RequestDestroyWindow();
		return FReply::Handled();
	}

	FReply OnCancel()
	{
		RequestDestroyWindow();
		return FReply::Handled();
	}

	void OnGroomsSelected(const FAssetData& SelectedAsset)
	{
		AddSkeletalsToCharacterCreationGroomsOptions.Grooms.Empty();
		TArray<FAssetData> SelectedAssets = GroomsCurrentSelectionDelegate.Execute();
		for (const FAssetData& Asset : SelectedAssets)
		{
			AddSkeletalsToCharacterCreationGroomsOptions.Grooms.Add(Cast<UCharacterCreatorGroom>(Asset.GetAsset()));
		}
	}

	void OnMeshesSelected(const FAssetData& SelectedAsset)
	{
		AddSkeletalsToCharacterCreationGroomsOptions.Meshes.Empty();
		TArray<FAssetData> SelectedAssets = MeshesCurrentSelectionDelegate.Execute();
		for (const FAssetData& Asset : SelectedAssets)
		{
			AddSkeletalsToCharacterCreationGroomsOptions.Meshes.Add(Cast<USkeletalMesh>(Asset.GetAsset()));
		}
	}

	void OnSourceMeshSelected(const FAssetData& SelectedAsset)
	{
		AddSkeletalsToCharacterCreationGroomsOptions.SourceMesh = Cast<USkeletalMesh>(SelectedAsset.GetAsset());
	}

private:
	FAddSkeletalsToCCGroomsOptions AddSkeletalsToCharacterCreationGroomsOptions;
	FGetCurrentSelectionDelegate GroomsCurrentSelectionDelegate;
	FGetCurrentSelectionDelegate MeshesCurrentSelectionDelegate;
};


void FCCEditAction_AddSkeletalsToCCGrooms::ExecuteAction()
{
	TSharedPtr<SWindow> PickerWindow = SNew(SAddSkeletalsToCharacterCreationGroomsWindow);
	GEditor->EditorAddModalWindow(PickerWindow.ToSharedRef());
	PickerWindow.Reset();
}


#undef LOCTEXT_NAMESPACE
