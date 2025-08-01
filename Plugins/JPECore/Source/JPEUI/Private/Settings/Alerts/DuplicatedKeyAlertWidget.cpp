// Created by Jorge Puerto. All Rights Reserved.


#include "Settings/Alerts/DuplicatedKeyAlertWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UDuplicatedKeyAlertWidget::SetKeyName(const FText& NewKeyName) const
{
	if (KeyName)
	{
		KeyName->SetText(NewKeyName);
	}
}

void UDuplicatedKeyAlertWidget::SetActionName(const FText& NewActionName) const
{
	if (ActionName)
	{
		ActionName->SetText(NewActionName);
	}
}

void UDuplicatedKeyAlertWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RevertButton)
	{
		RevertButton->OnClicked.AddUniqueDynamic(this, &UDuplicatedKeyAlertWidget::HandleOnRevertButtonClicked);
	}
	if (KeepButton)
	{
		KeepButton->OnClicked.AddUniqueDynamic(this, &UDuplicatedKeyAlertWidget::HandleOnKeepButtonClicked);
	}
}

void UDuplicatedKeyAlertWidget::HandleOnRevertButtonClicked()
{
	if (OnRevertButtonClicked.IsBound())
	{
		OnRevertButtonClicked.Execute();
	}
	RemoveFromParent();
}

void UDuplicatedKeyAlertWidget::HandleOnKeepButtonClicked()
{
	if (OnKeepButtonClicked.IsBound())
	{
		OnKeepButtonClicked.Execute();
	}
	RemoveFromParent();
}
