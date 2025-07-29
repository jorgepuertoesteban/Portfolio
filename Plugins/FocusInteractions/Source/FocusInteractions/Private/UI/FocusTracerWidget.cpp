// Created by Jorge Puerto. All Rights Reserved.


#include "UI/FocusTracerWidget.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Components/FocusTracerComponent.h"
#include "FocusInteractionsTypes.h"
#include "Components/PanelWidget.h"
#include "UI/ActionByActionsSetResultWidget.h"


bool UFocusTracerWidget::Initialize()
{
	if (Super::Initialize())
	{
		Configure();
		return true;
	}
	return false;
}

void UFocusTracerWidget::NewFocus(const FFocusInfo& Info)
{
	for (const FActionByActionsSetResult& ActionResult : Info.Actions)
	{
		TSubclassOf<UActionByActionsSetResultWidget> Class = ActionByActionsSetResultWidgetClasses.IsValidIndex(ActionResult.ActionSetIndex) ? ActionByActionsSetResultWidgetClasses[ActionResult.ActionSetIndex]: DefaultActionByActionsSetResultWidgetClass;
		if (!Class)
		{
			UE_LOG(LogTemp, Error, TEXT("Class not found for ActionSetIndex %i"), ActionResult.ActionSetIndex);
			continue;
		}

		UActionByActionsSetResultWidget* NewWidget = CreateWidget<UActionByActionsSetResultWidget>(GetOwningPlayer(), Class);
		if (!NewWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("Class Couldn't be created for ActionSetIndex %i"), ActionResult.ActionSetIndex);
			continue;
		}

		NewWidget->SetActionByActionsSetResult(ActionResult);
		ActionsPanel->AddChild(NewWidget);
	}
	OnNewTracerFocus(Info);
}

void UFocusTracerWidget::EndFocus(const UFocusableComponent* Focusable)
{
	ClearPanel();
	OnEndTracerFocus(Focusable);
}

void UFocusTracerWidget::Configure()
{
	AActor* const ActorWithFocusTracer = bUsePawn ? Cast<AActor>(GetOwningPlayerPawn()) : Cast<AActor>(GetOwningPlayer());

	if (ActorWithFocusTracer)
	{
		if (UFocusTracerComponent* FocusTracer = Cast<UFocusTracerComponent>(ActorWithFocusTracer->GetComponentByClass(UFocusTracerComponent::StaticClass())))
		{
			FocusTracer->OnNewFocus.AddUniqueDynamic(this, &UFocusTracerWidget::NewFocus);
			FocusTracer->OnEndFocus.AddUniqueDynamic(this, &UFocusTracerWidget::EndFocus);
		}
	}
}

void UFocusTracerWidget::ClearPanel()
{
	ActionsPanel->ClearChildren();
}
