// Created by Jorge Puerto. All Rights Reserved.

#include "CreateFactoryWindow.h"

#include "CreateFactoryCommand.h"
#include "CreateFactoryOptions.h"
#include "Editor/EditorEngine.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Text/STextBlock.h"

#include "UObject/ReflectedTypeAccessors.h"

#define LOCTEXT_NAMESPACE "SCreateFactoryWindow"

#if ENGINE_MAJOR_VERSION == 4
using FMyStyle = FEditorStyle;
#elif ENGINE_MAJOR_VERSION == 5
using FMyStyle = FAppStyle;
#endif


void SCreateFactoryWindow::Construct(const FArguments& InArgs)
{
	for (const EFactoryType FactoryType : TEnumRange<EFactoryType>())
	{
		FactoryTypeOptions.Add(MakeShared<EFactoryType>(FactoryType));
	}

	SEditableTextBox EditableTextBox1;

	SWindow::Construct(
		SWindow::FArguments()
		.Title(LOCTEXT("SCreateFactoryWindowTitle", "Create Factory"))
		.SupportsMinimize(false)
		.SupportsMaximize(false)
		.ClientSize(FVector2D(500, 300))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot() // TODO: Replace with plugin selector
			[
				SNew(SEditableTextBox)
				.HintText(LOCTEXT("SCreateFactoryWindow_Tooltip_PluginName", "Plugin Name"))
				.OnTextChanged_Static(&SCreateFactoryWindow::OnTextChanged, &CreateFactoryOptions.PluginName)
			]
			+ SVerticalBox::Slot() // TODO: Replace with plugin selector
			[
				SNew(SEditableTextBox)
				.HintText(LOCTEXT("SCreateFactoryWindow_Tooltip_ModuleName", "Module Name"))
				.OnTextChanged_Static(&SCreateFactoryWindow::OnTextChanged, &CreateFactoryOptions.ModuleName)
			]
			+ SVerticalBox::Slot() // TODO: Replace with .h file selector
			[
				SNew(SEditableTextBox)
				.HintText(LOCTEXT("SCreateFactoryWindow_Tooltip_FactoryName", "Asset Name for the Factory"))
				.OnTextChanged_Static(&SCreateFactoryWindow::OnTextChanged, &CreateFactoryOptions.AssetNameForTheFactory)
			]
			+ SVerticalBox::Slot()
			[
				SNew(SComboBox<FFactoryType>)
				.OptionsSource(&FactoryTypeOptions)
				.OnSelectionChanged_Static(&SCreateFactoryWindow::OnSelectionChanged, &CurrentFactoryType, &CreateFactoryOptions.FactoryType)
				.OnGenerateWidget_Static(&SCreateFactoryWindow::MakeWidgetForOption)
				.InitiallySelectedItem(CurrentFactoryType)
				[
					SNew(STextBlock)
					.Text(this, &SCreateFactoryWindow::GetCurrentItemLabel)
				]
			]
			// TODO: Path selector where we want to place the newly created directory
			+ SVerticalBox::Slot()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged_Static(&SCreateFactoryWindow::OnChecked, &CreateFactoryOptions.bReplaceFileIfExists)
				.IsChecked_Static(&SCreateFactoryWindow::IsChecked, &CreateFactoryOptions.bReplaceFileIfExists)
				.ToolTipText(LOCTEXT("SCreateFactoryWindow_Tooltip_ReplaceFileIfExists", "Replaces files in the case they already exist"))
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SCreateFactoryWindow_Checkbox_ReplaceFileIfExists", "Replace files if they already exist"))
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			[
				SAssignNew(MessageField, STextBlock).Text(FText::FromString("")).ColorAndOpacity(FSlateColor(FLinearColor::Red))
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
					.Text(LOCTEXT("Close", "Close"))
					.OnClicked(this, &SCreateFactoryWindow::OnCloseClick, EButtonValue::Cancel)
					.IsEnabled(true)
				]
				+ SUniformGridPanel::Slot(1, 0)
				.HAlign(HAlign_Left)
				[
					SNew(SButton)
					.HAlign(HAlign_Left)
					.ContentPadding(FMyStyle::GetMargin("StandardDialog.ContentPadding"))
					.Text(LOCTEXT("Accept", "Accept"))
					.OnClicked(this, &SCreateFactoryWindow::OnButtonClick, EButtonValue::Accept)
					.IsEnabled(true)
				]
			]
		]
	);
}

bool SCreateFactoryWindow::ShowModal(FCreateFactoryOptions& OutCreateFactoryOptions)
{
	OutCreateFactoryOptions = CreateFactoryOptions;
	GEditor->EditorAddModalWindow(SharedThis(this));
	return ButtonPressed == EButtonValue::Accept;
}

TSharedRef<SWidget> SCreateFactoryWindow::MakeWidgetForOption(FFactoryType InOption)
{
	return SNew(STextBlock).Text(StaticEnum<EFactoryType>()->GetDisplayValueAsText(*InOption.Get()));
}

void SCreateFactoryWindow::OnSelectionChanged(FFactoryType NewValue, ESelectInfo::Type, FFactoryType* Value, EFactoryType* FactoryValue)
{
	*Value = NewValue;
	*FactoryValue = *(Value->Get());
}

FText SCreateFactoryWindow::GetCurrentItemLabel() const
{
	if (!CurrentFactoryType.Get())
	{
		return LOCTEXT("SCreateFactoryWindow_Tooltip_FactoryType", "Factory Type");
	}
	return StaticEnum<EFactoryType>()->GetDisplayValueAsText(*CurrentFactoryType.Get());
}

FReply SCreateFactoryWindow::OnButtonClick(EButtonValue NewButtonPressed)
{
	ButtonPressed = NewButtonPressed;
	OnAcceptButtonClick();
	return FReply::Handled();
}

FReply SCreateFactoryWindow::OnCloseClick(EButtonValue NewButtonPressed)
{
	RequestDestroyWindow();
	return FReply::Handled();
}

bool SCreateFactoryWindow::OnAcceptButtonClick() const
{
	if (CreateFactoryOptions.ModuleName.TrimEnd().Equals("") ||
		CreateFactoryOptions.AssetNameForTheFactory.TrimEnd().Equals("") ||
		!CurrentFactoryType)
	{
		UE_LOG(LogTemp, Warning, TEXT("Please, fill in all the fields."));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Creating factory with the following parameters: "));
	UE_LOG(LogTemp, Warning, TEXT("ModuleName: %s"), *CreateFactoryOptions.ModuleName);
	UE_LOG(LogTemp, Warning, TEXT("FactoryName: %s"), *CreateFactoryOptions.AssetNameForTheFactory);
	UE_LOG(LogTemp, Warning, TEXT("FactoryType: %s"), *StaticEnum<EFactoryType>()->GetDisplayValueAsText(CreateFactoryOptions.FactoryType).ToString());
	UE_LOG(LogTemp, Warning, TEXT("TargetPath: %s"), *CreateFactoryOptions.TargetPath);

	FText Message;
	const bool bIsSuccessful = CreateFactoryCommand::Run(CreateFactoryOptions);
	if (bIsSuccessful)
	{
		Message = FText::FromString("Factory Created");
	}
	else
	{
		Message = FText::FromString("Failed!");
	}
	if (MessageField.IsValid())
	{
		MessageField->SetText(Message);
	}
	return bIsSuccessful;
}

void SCreateFactoryWindow::OnTextChanged(const FText& NewText, FString* Value)
{
	*Value = NewText.ToString();
}

ECheckBoxState SCreateFactoryWindow::IsChecked(bool* Value)
{
	return *Value ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SCreateFactoryWindow::OnChecked(ECheckBoxState NewCheckedState, bool* Value)
{
	*Value = NewCheckedState == ECheckBoxState::Checked;
}

#undef LOCTEXT_NAMESPACE
