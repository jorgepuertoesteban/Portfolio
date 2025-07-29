// Created by Jorge Puerto. All Rights Reserved.


#include "UI/ActionByActionsSetResultWidget.h"
#include "Components/TextBlock.h"
#include "ActionType.h"

bool UActionByActionsSetResultWidget::Initialize()
{
	if (Super::Initialize() && ActionTextBlock)
	{
		Refresh();
		return true;
	}
	return false;
}

void UActionByActionsSetResultWidget::SetActionByActionsSetResult(const FActionByActionsSetResult& NewActionByActionsSetResult)
{
	ActionByActionsSetResult = NewActionByActionsSetResult;
	Refresh();
}

void UActionByActionsSetResultWidget::Refresh()
{
	if (ActionTextBlock && ActionByActionsSetResult.Action)
	{
		ActionTextBlock->SetText(ActionByActionsSetResult.Action->GetActionFriendlyName(ActionByActionsSetResult.ActionableActor));
	}
}
