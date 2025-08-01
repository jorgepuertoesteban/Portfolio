// Created by Jorge Puerto. All Rights Reserved.

#include "UI/Elements/DisButton.h"
#include "Components/ButtonSlot.h"
#include "UI/Elements/SDisButton.h"
#include "Components/PanelWidget.h"
#include "Slate/ButtonStyleAsset.h"

TSharedRef<SWidget> UDisButton::RebuildWidget()
{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	TSharedPtr<USDisButton> MyDisButton = SNew(USDisButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
		.ButtonStyle(&GetStyle())
		.ClickMethod(GetClickMethod())
		.TouchMethod(GetTouchMethod())
		.PressMethod(GetPressMethod())
		.IsFocusable(GetIsFocusable())
		;
PRAGMA_ENABLE_DEPRECATION_WARNINGS
	
	MyButton = MyDisButton;
	MyDisButton->FocusSlateBrushAsset = FocusSlateBrushAsset;

	if (GetChildrenCount() > 0)
	{
		Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}

	RebuildWidgetBP();

	return MyButton.ToSharedRef();
}

UDisButton::UDisButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (bUseJPEStyleAsset && JPEButtonStyleAsset)
	{
		SetStyle(JPEButtonStyleAsset->ButtonStyle);
	}
}