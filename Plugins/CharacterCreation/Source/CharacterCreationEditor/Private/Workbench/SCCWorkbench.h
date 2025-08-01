// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "SCCWorkbenchViewport.h"
#include "UObject/GCObject.h"
#include "Widgets/SCompoundWidget.h"

class CHARACTERCREATIONEDITOR_API SCCWorkbench : public SCompoundWidget
{

	SLATE_BEGIN_ARGS(SCCWorkbench)
	{}
	
	SLATE_END_ARGS()
	
private:

	TSharedPtr<class SCCWorkbenchViewport> PreviewViewport;

public:

	void Construct(const FArguments& InArgs);

};