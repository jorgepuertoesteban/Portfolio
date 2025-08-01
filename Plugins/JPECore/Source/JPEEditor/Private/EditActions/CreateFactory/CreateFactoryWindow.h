// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CreateFactoryOptions.h"
#include "Input/Reply.h"
#include "Widgets/SWindow.h"


class SCreateFactoryWindow : public SWindow
{
	enum EButtonValue
	{
		Cancel,
		Accept
	};

	typedef TSharedPtr<EFactoryType> FFactoryType;
	typedef TSharedPtr<FString> FPluginType;

private:
	TSharedPtr<STextBlock> MessageField; // Store a reference to the message text block

public:
	FCreateFactoryOptions CreateFactoryOptions;

	TArray<FFactoryType> FactoryTypeOptions;
	FFactoryType CurrentFactoryType;

	TArray<FPluginType> PluginTypeOptions;
	FPluginType CurrentPluginType;


	EButtonValue ButtonPressed;

public:
	void Construct(const FArguments& InArgs);

	bool ShowModal(FCreateFactoryOptions& OutCreateFactoryOptions);

	static TSharedRef<SWidget> MakeWidgetForOption(FFactoryType InOption);

	static void OnSelectionChanged(FFactoryType NewValue, ESelectInfo::Type, FFactoryType* Value, EFactoryType* FactoryValue);

	FText GetCurrentItemLabel() const;

	FReply OnButtonClick(EButtonValue NewButtonPressed);

	FReply OnCloseClick(EButtonValue NewButtonPressed);

	bool OnAcceptButtonClick() const;

	static void OnTextChanged(const FText& NewText, FString* Value);

	static ECheckBoxState IsChecked(bool* Value);

	static void OnChecked(ECheckBoxState NewCheckedState, bool* Value);
};
