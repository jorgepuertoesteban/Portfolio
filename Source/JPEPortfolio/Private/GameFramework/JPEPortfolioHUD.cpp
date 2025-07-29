// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/JPEPortfolioHUD.h"
#include "UI/FocusTracerWidget.h"


AJPEPortfolioHUD::AJPEPortfolioHUD() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AJPEPortfolioHUD::ShowFocusTraceInterface()
{
	if (FocusTracerPawnWidget)
	{
		FocusTracerPawnWidget->RemoveFromParent();
	}
	FocusTracerPawnWidget = CreateWidget<UFocusTracerWidget>(GetOwningPlayerController(), FocusTracerPawnWidgetClass);
	FocusTracerPawnWidget->AddToViewport();
}

void AJPEPortfolioHUD::HideFocusTraceInterface()
{
	if (FocusTracerPawnWidget)
	{
		FocusTracerPawnWidget->RemoveFromParent();
		FocusTracerPawnWidget = nullptr;
	}
}

