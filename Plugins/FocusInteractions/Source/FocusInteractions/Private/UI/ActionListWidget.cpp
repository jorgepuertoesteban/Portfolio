// Created by Jorge Puerto. All Rights Reserved.


#include "UI/ActionListWidget.h"
#include "Components/ListView.h"
//#include "Interfaces/InventoryOwner.h"
//#include "PlayerCombatPawn.h"
//#include "Inventory/InventoryItem.h"
#include "ActionsSet.h"
#include "Components/FocusableComponent.h"
#include "ActionType.h"
#include "UI/ActionListItemWidget.h"
#include "FocusInteractionsTypes.h"
#include "UI/ActionListInfo.h"

bool UActionListWidget::Initialize()
{
	return Super::Initialize();
}

void UActionListWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
}

bool UActionListWidget::RefreshList()
{
	TArray<UActionListInfo*> ActionsListInfo;
	for (int i = 0; i < Info.Actions.Num(); i++)
	{
		ActionsListInfo.Add(UActionListInfo::NEW(Info, i));
	}

	ActionsListView->ClearListItems();
	ActionsListView->SetListItems(ActionsListInfo);

	ActionsListView->OnEntryWidgetGenerated().AddUObject(this, &UActionListWidget::WidgetLoaded);

	SetVisibility(ESlateVisibility::Visible);
	return true;
}

void UActionListWidget::Configure(const FFocusInfo& NewInfo)
{
	Info = NewInfo;
}

void UActionListWidget::Close()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UActionListWidget::WidgetLoaded(UUserWidget& NewWidget)
{
	UActionListItemWidget* Widget = Cast<UActionListItemWidget>(&NewWidget);
	if (Widget == nullptr)
	{
		return;
	}
	Widget->OnClicked.AddUniqueDynamic(this, &UActionListWidget::Close);
}

void UActionListWidget::OnVisibilityAnimEnd_Implementation()
{
	if (!bWantsToBeVisible)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UActionListWidget::OnVisibilityAnimStart_Implementation()
{
	if (bWantsToBeVisible)
	{
		RefreshList();
		SetVisibility(ESlateVisibility::Visible);
		//Llamar a una funcion que cargue los widgets en la lista.
	}
}
