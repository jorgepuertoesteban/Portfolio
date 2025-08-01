// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWindow.h"
#include "ExtractOptions.h"



class SMeshFromBlueprint : public SWindow
{

	enum EButtonValue
	{
		Cancel,
		Accept
	};

public:

	FExtractOptions ExtractOptions;
	EButtonValue ButtonPressed;

public:

	void Construct(const FArguments& InArgs);

	static ECheckBoxState IsChecked(bool* Value);
	
	static void OnChecked(ECheckBoxState NewCheckedState, bool* Value);

	/** Displays the dialog in a blocking fashion */
	bool ShowModal(FExtractOptions& OutExtractOptions);

	FReply OnButtonClick(EButtonValue NewButtonPressed);
};