// Created by Jorge Puerto. All Rights Reserved.

#include "UI/Elements/SDisButton.h"
#include "Slate/SlateBrushAsset.h"


const FSlateBrush* USDisButton::GetFocusBrush() const
{
	return FocusSlateBrushAsset ? &FocusSlateBrushAsset->Brush : SButton::GetFocusBrush();
}

FReply USDisButton::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	PlayHoverSound();
	return SButton::OnFocusReceived(MyGeometry, InFocusEvent);
}
