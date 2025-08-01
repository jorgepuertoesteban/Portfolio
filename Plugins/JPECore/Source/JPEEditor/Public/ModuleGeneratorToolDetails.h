// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"
#include "ModuleEditorTool.h"
#include "DetailWidgetRow.h"

class JPEEDITOR_API FModuleGeneratorToolDetails : public IDetailCustomization
{
public:
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	static TSharedRef<IDetailCustomization> MakeInstance();
	typedef TSharedPtr<FString> FComboItemType;

	//Ref to the current tool instance
	TWeakObjectPtr<UModuleEditorTool> Tool;


	//Plugin ComboBox Data/Events
	FComboItemType CurrentSelectedPlugin;
	TArray<FComboItemType> Options;
	FText GetCurrentItemLabel() const;
	void OnSelectionChanged(FComboItemType NewValue, ESelectInfo::Type);
	TSharedRef<SWidget> MakeWidgetForOption(FComboItemType InOption);

	
	void OnTemplateChange();
	void FillComboBoxWithPluginNames();
	EVisibility GetBasicParentClassVisibility() const;
	FReply GenerateNewModuleCall();


	EVisibility GetNameErrorLabelVisibility() const;
	FText GetNameErrorLabelText() const;
	void OnChangeNewModuleName();
	
	FString LastErrorText;
	bool WasError = false;
};