// Created by Jorge Puerto. All Rights Reserved.

#include "EditActions/SMeshFromBlueprint.h"
#include "Editor/EditorEngine.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "SMeshFromBlueprints"

#if ENGINE_MAJOR_VERSION == 4
using FMyStyle = FEditorStyle;
#elif ENGINE_MAJOR_VERSION == 5
using FMyStyle = FAppStyle;
#endif


void SMeshFromBlueprint::Construct(const FArguments& InArgs)
{
	SWindow::Construct(
		SWindow::FArguments()
		.Title(LOCTEXT("SMeshFromBlueprintsTitle", "Extract Options"))
		.SupportsMinimize(false)
		.SupportsMaximize(false)
		.ClientSize(FVector2D(900, 500))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged_Static(&SMeshFromBlueprint::OnChecked, &ExtractOptions.bSpawnOneActorPerComponent /*, ExtractOptions.bSpawnOneActorPerComponent*/)
				.IsChecked_Static(&SMeshFromBlueprint::IsChecked, &ExtractOptions.bSpawnOneActorPerComponent)
				.ToolTipText(LOCTEXT("SMeshFromBlueprints_Tooltip_SpawnOneActorPerComponent", "Spawns one actor per component"))
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SMeshFromBlueprints_Checkbox_SpawnOneActorPerComponent", "Spawn one actor per component"))
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged_Static(&SMeshFromBlueprint::OnChecked, &ExtractOptions.bUseLevelNameAsRootFolder)
				.IsChecked_Static(&SMeshFromBlueprint::IsChecked, &ExtractOptions.bUseLevelNameAsRootFolder)
				.ToolTipText(LOCTEXT("SMeshFromBlueprints_Tooltip_UseLevelNameAsRootFolder", "Use level name as root folder"))
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SMeshFromBlueprints_Checkbox_UseLevelNameAsRootFolder", "Use level name as root folder"))
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged_Static(&SMeshFromBlueprint::OnChecked, &ExtractOptions.bIterateAllActors)
				.IsChecked_Static(&SMeshFromBlueprint::IsChecked, &ExtractOptions.bIterateAllActors)
				.ToolTipText(LOCTEXT("SMeshFromBlueprints_Tooltip_IterateAllActors", "Iterate all actors"))
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SMeshFromBlueprints_Checkbox_IterateAllActors", "Iterate all actors"))
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged_Static(&SMeshFromBlueprint::OnChecked, &ExtractOptions.bSkipActorReplacementConfirmation)
				.IsChecked_Static(&SMeshFromBlueprint::IsChecked, &ExtractOptions.bSkipActorReplacementConfirmation)
				.ToolTipText(LOCTEXT("SMeshFromBlueprints_Tooltip_bSkipActorReplacementConfirmation", "Skip actor replacement confirmation"))
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SMeshFromBlueprints_Checkbox_bSkipActorReplacementConfirmation", "Skip actor replacement confirmation"))
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged_Static(&SMeshFromBlueprint::OnChecked, &ExtractOptions.bForceStatic)
				.IsChecked_Static(&SMeshFromBlueprint::IsChecked, &ExtractOptions.bForceStatic)
				.ToolTipText(LOCTEXT("SMeshFromBlueprints_Tooltip_ForceStatic", "Force static mobility to each Component"))
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SMeshFromBlueprints_Checkbox_ForceStatic", "Force static mobility to each Component"))
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged_Static(&SMeshFromBlueprint::OnChecked, &ExtractOptions.bOnlyConstructorComponentsActors)
				.IsChecked_Static(&SMeshFromBlueprint::IsChecked, &ExtractOptions.bOnlyConstructorComponentsActors)
				.ToolTipText(LOCTEXT("SMeshFromBlueprints_Tooltip_OnlyConstructorComponentsActors", "Only actors with Components spawned in the Blueprint's constructor"))
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SMeshFromBlueprints_Checkbox_OnlyConstructorComponentsActors", "Only actors with components spawned in the Blueprint's constructor"))
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged_Static(&SMeshFromBlueprint::OnChecked, &ExtractOptions.bDeleteOriginal)
				.IsChecked_Static(&SMeshFromBlueprint::IsChecked, &ExtractOptions.bDeleteOriginal)
				.ToolTipText(LOCTEXT("SMeshFromBlueprints_Tooltip_bDeleteOriginal", "Delete original Actor"))
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SMeshFromBlueprints_Checkbox_bDeleteOriginal", "Delete original Actor"))
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged_Static(&SMeshFromBlueprint::OnChecked, &ExtractOptions.bSkipDeleteConfirmation)
				.IsChecked_Static(&SMeshFromBlueprint::IsChecked, &ExtractOptions.bSkipDeleteConfirmation)
				.ToolTipText(LOCTEXT("SMeshFromBlueprints_Tooltip_bSkipDeleteConfirmation", "Skip Delete Confirmation"))
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SMeshFromBlueprints_Checkbox_bSkipDeleteConfirmation", "Skip Delete Confirmation"))
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged_Static(&SMeshFromBlueprint::OnChecked, &ExtractOptions.bReplaceAlreadyExtracted)
				.IsChecked_Static(&SMeshFromBlueprint::IsChecked, &ExtractOptions.bReplaceAlreadyExtracted)
				.ToolTipText(LOCTEXT("SMeshFromBlueprints_Tooltip_bReplaceAlreadyExtracted", "Extract actors already extracted"))
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SMeshFromBlueprints_Checkbox_bReplaceAlreadyExtracted", "Extract actors already extracted"))
				]
			]
			//
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Right)
			.Padding(5)
			[
				SNew(SUniformGridPanel)
				.SlotPadding(FMyStyle::GetMargin("StandardDialog.SlotPadding"))
				.MinDesiredSlotWidth(FMyStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
				.MinDesiredSlotHeight(FMyStyle::GetFloat("StandardDialog.MinDesiredSlotHeight"))
				+ SUniformGridPanel::Slot(0, 0)
				.HAlign(HAlign_Left)
				[
					SNew(SButton)
					.HAlign(HAlign_Left)
					.ContentPadding(FMyStyle::GetMargin("StandardDialog.ContentPadding"))
					.Text(LOCTEXT("Cancel", "Cancel"))
					.OnClicked(this, &SMeshFromBlueprint::OnButtonClick, EButtonValue::Cancel)
					.IsEnabled(true)
				]
				+ SUniformGridPanel::Slot(1, 0)
				.HAlign(HAlign_Left)
				[
					SNew(SButton)
					.HAlign(HAlign_Left)
					.ContentPadding(FMyStyle::GetMargin("StandardDialog.ContentPadding"))
					.Text(LOCTEXT("Accept", "Accept"))
					.OnClicked(this, &SMeshFromBlueprint::OnButtonClick, EButtonValue::Accept)
					.IsEnabled(true)
				]
			]
		]
	);
}

ECheckBoxState SMeshFromBlueprint::IsChecked(bool* Value)
{
	return *Value ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SMeshFromBlueprint::OnChecked(ECheckBoxState NewCheckedState, bool* Value)
{
	*Value = NewCheckedState == ECheckBoxState::Checked;
}

bool SMeshFromBlueprint::ShowModal(FExtractOptions& OutExtractOptions)
{
	OutExtractOptions = ExtractOptions;
	GEditor->EditorAddModalWindow(SharedThis(this));
	return ButtonPressed == EButtonValue::Accept;
}


FReply SMeshFromBlueprint::OnButtonClick(EButtonValue NewButtonPressed)
{
	ButtonPressed = NewButtonPressed;
	RequestDestroyWindow();
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE