// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SViewportToolBar.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "Widgets/Input/SSpinBox.h"

class SMeshMorpherPreviewViewport;
class SSlider;


class CHARACTERCREATIONEDITOR_API SCCWorkbenchViewportToolbar : public SViewportToolBar
{
public:
	SLATE_BEGIN_ARGS(SCCWorkbenchViewportToolbar) {}
	SLATE_ARGUMENT(TSharedPtr<class SCCWorkbenchViewport>, Viewport)
		SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};