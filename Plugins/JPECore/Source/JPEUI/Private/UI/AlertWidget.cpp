// Created by Jorge Puerto. All Rights Reserved.


#include "UI/AlertWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

bool UAlertWidget::Initialize()
{
	if (Super::Initialize() && AcceptButton && BodyText)
	{
		AcceptButton->OnClicked.AddDynamic(this, &UAlertWidget::OnAcceptButtonClicked);
		return true;
	}
	return false;
}

void UAlertWidget::SetBodyText(const FText& NewText)
{
	BodyText->SetText(NewText);
}

void UAlertWidget::OnAcceptButtonClicked()
{
	RemoveFromParent();
}

void UAlertWidget::HideAcceptButton() const
{
	AcceptButton->SetVisibility(ESlateVisibility::Collapsed);
}
