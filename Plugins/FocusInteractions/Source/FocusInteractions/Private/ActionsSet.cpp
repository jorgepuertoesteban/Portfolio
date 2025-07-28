// Created by Bionic Ape. All rights reserved.

#include "ActionsSet.h"
#include "ActionableType.h"
#include "ActionType.h"

UActionType* UActionsSet::GetActionType(UActionableType* ActionableType, float Distance) const
{
	//if (ActionableType && Actions.Contains(ActionableType))
	//{
	//	if (UActionType* ActionType = Actions[ActionableType])
	//	{
	//		if (Distance < ActionType->GetMaxDistance())
	//		{
	//			return ActionType;
	//		}
	//	}
	//}
	//return nullptr;

	if (UActionType* ActionType = GetActionType(ActionableType))
	{
		if (Distance < ActionType->GetMaxDistance())
		{
			return ActionType;
		}
	}
	return nullptr;
}

UActionType* UActionsSet::GetActionType(UActionableType* ActionableType) const
{
	if (ActionableType && Actions.Contains(ActionableType))
	{
		if (UActionType* ActionType = Actions[ActionableType])
		{
			return ActionType;
		}
	}
	return nullptr;
}

TArray<UActionType*> UActionsSet::GetAllActions() const
{
	TArray<UActionType*> AllActions;
	for (auto& Elem : Actions)
	{
		AllActions.Add(Elem.Value);
	}
	return AllActions;
}
