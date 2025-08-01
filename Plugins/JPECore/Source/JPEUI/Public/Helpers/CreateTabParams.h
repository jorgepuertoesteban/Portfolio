// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateTabParams.generated.h"

class UCategoryTabWidget;
class UObject;
class UWidget;
class UHorizontalBox;
class UTextBlock;
class UWidgetSwitcher;

USTRUCT(BlueprintType)
struct JPEUI_API FCreateCategoryTabParams
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCategoryTabWidget* TabWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UWidget* ContentWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FText TabCategoryName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UObject* ResourceObject;


	FCreateCategoryTabParams() :
		TabWidget(nullptr),
		ContentWidget(nullptr),
		ResourceObject(nullptr)
	{

	}

	FCreateCategoryTabParams(
		UWidget* NewContentWidget,
		FText NewTabCategoryName,
		UObject* NewResourceObject) :
		ContentWidget(NewContentWidget),
		TabCategoryName(NewTabCategoryName),
		ResourceObject(NewResourceObject)
	{
	}
};