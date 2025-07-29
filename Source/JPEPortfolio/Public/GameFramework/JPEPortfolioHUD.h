// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interfaces/FocusTracerHUDInterface.h"
#include "JPEPortfolioHUD.generated.h"

class UFocusTracerWidget;

UCLASS()
class JPEPORTFOLIO_API AJPEPortfolioHUD : 
	public AHUD,
	public IFocusTracerHUDInterface

{
	GENERATED_BODY()


public:
	AJPEPortfolioHUD();

	void CreateWidgetsPossessedPawn();

	
#pragma region IFocusTracerHUDInterface

protected:
	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TSubclassOf<UUserWidget> FocusTracerPawnWidgetClass;
	UPROPERTY(Transient, BlueprintReadOnly, Category = Widgets)
	UFocusTracerWidget* FocusTracerPawnWidget;

public:
	virtual void HideFocusTraceInterface() override;

#pragma endregion
};
