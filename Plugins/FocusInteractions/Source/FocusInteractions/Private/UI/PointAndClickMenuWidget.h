// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PointAndClickMenuWidget.generated.h"

UCLASS()
class FOCUSINTERACTIONS_API UPointAndClickMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void SelectIndexAction(int32 Index);
};
