// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "Widgets/Input/SButton.h"

class USlateBrushAsset;

class JPEUI_API USDisButton : public SButton
{

public:

	UPROPERTY(Transient)
	USlateBrushAsset* FocusSlateBrushAsset;

	virtual const FSlateBrush* GetFocusBrush() const override;

	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
};

