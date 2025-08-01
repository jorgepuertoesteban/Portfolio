// Created by Jorge Puerto. All Rights Reserved.


#include "MultiplayerHUD.h"
#include "Blueprint/UserWidget.h"


void AMultiplayerHUD::BeginPlay()
{
	Super::BeginPlay();
	LoadMenu();
}

void AMultiplayerHUD::LoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return;

	UUserWidget* Menu = CreateWidget<UUserWidget>(GetOwningPlayerController(), MenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->AddToViewport();
}

void AMultiplayerHUD::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;

	UUserWidget* Menu = CreateWidget<UUserWidget>(GetOwningPlayerController(), InGameMenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->AddToViewport();
}