// Created by Jorge Puerto. All Rights Reserved.


#include "UI/ConfirmWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

bool UConfirmWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (!AcceptButton) return false;
		if (!BodyText) return false;
		if (!CancelButton) return false;
		
		AcceptButton->OnClicked.AddDynamic(this, &UConfirmWidget::OnAcceptButtonClicked);
		CancelButton->OnClicked.AddDynamic(this, &UConfirmWidget::OnCancelButtonClicked);
	}
	return false;
}

void UConfirmWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetUserFocus(GetOwningPlayer());
}

FReply UConfirmWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (KeysToCloseDialog.Contains(InKeyEvent.GetKey()))
	{
		OnCancelButtonClicked();
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UConfirmWidget::SetBodyText(const FText& NewText)
{
	BodyText->SetText(NewText);
}

void UConfirmWidget::OnAcceptButtonClicked()
{
	OnFinishConfirmWidget.ExecuteIfBound(true);
	RemoveFromParent();
}

void UConfirmWidget::OnCancelButtonClicked()
{
	OnFinishConfirmWidget.ExecuteIfBound(false);
	RemoveFromParent();
}
