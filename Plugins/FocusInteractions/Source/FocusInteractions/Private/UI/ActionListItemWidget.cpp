// Created by Jorge Puerto. All Rights Reserved.


#include "UI/ActionListItemWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/ActionListWidget.h"
#include "Components/FocusComponent.h"
#include "UI/ActionListInfo.h"


bool UActionListItemWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (ActionButton)
		{
			ActionButton->OnClicked.AddUniqueDynamic(this, &UActionListItemWidget::OnButtonClicked);
			return true;
		}
	}
	return false;
}

void UActionListItemWidget::OnButtonClicked()
{
	UWidgetBlueprintLibrary::SetFocusToGameViewport();
	ActionInfo->Info.FocusComponent->FocusInfo = ActionInfo->Info;
	ActionInfo->Info.FocusComponent->PerformAction(ActionInfo->Index);
	OnClicked.Broadcast();
}

void UActionListItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	ActionInfo = Cast<UActionListInfo>(ListItemObject);
}

void UActionListItemWidget::SetIsButtonEnabled(bool bIsButtonEnabled)
{
	if (ActionButton)
	{
		ActionButton->SetIsEnabled(bIsButtonEnabled);
	}
}
