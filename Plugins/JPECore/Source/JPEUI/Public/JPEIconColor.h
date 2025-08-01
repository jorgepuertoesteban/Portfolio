// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JPEIconColor.generated.h"


UENUM(BlueprintType)
enum class EIconBaseType : uint8
{
    SQUARE  			UMETA(DisplayName = "Square"),
    SQUARE45		    UMETA(DisplayName = "Square45"),
    ELLIPSE			    UMETA(DisplayName = "Ellipse"),
    SQUARE_STRETCHED    UMETA(DisplayName = "SquareStretched"),
    CUSTOM				UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EIconSize : uint8
{
	ICON_SMALL  			UMETA(DisplayName = "64 x 64"),
    ICON_BIG		        UMETA(DisplayName = "256 x 256")
};

UENUM(BlueprintType)
enum class EGlassMaskType : uint8
{
	ICON_MASK1    			UMETA(DisplayName = "Mask1"),
    ICON_MASK2              UMETA(DisplayName = "Mask2")
};

/**
 *
 */
UCLASS(hidecategories = Object, BlueprintType, Category = "Icon")
class JPEUI_API UJPEIconColor : public UObject
{
    GENERATED_BODY()
public:

    UPROPERTY(Category = Icon, EditAnywhere, BlueprintReadOnly)
    FLinearColor IconColor;

    UPROPERTY(Category = Icon, EditAnywhere, BlueprintReadOnly)
    float IconScale = 1.f;

    UPROPERTY(Category = Icon, EditAnywhere, BlueprintReadOnly)
    float OverallAngle;

    UPROPERTY(Category = Icon, EditAnywhere, BlueprintReadOnly)
    EIconBaseType IconBaseType = EIconBaseType::SQUARE;

    UPROPERTY(Category = Icon, EditAnywhere, BlueprintReadOnly)
    EIconSize IconSize = EIconSize::ICON_BIG;

	UPROPERTY(Category = Icon, EditAnywhere, BlueprintReadOnly)
	float IconTranslationX = 0.f;

	UPROPERTY(Category = Icon, EditAnywhere, BlueprintReadOnly)
	float IconTranslationY = 0.f;

    UPROPERTY(Category = Colors, EditAnywhere, BlueprintReadOnly)
    bool bUseBaseColor = true;

    UPROPERTY(Category = Colors, EditAnywhere, BlueprintReadOnly)
    FLinearColor BaseColor;

    UPROPERTY(Category = Colors, EditAnywhere, BlueprintReadOnly)
    bool bUseBaseMask = true;

    UPROPERTY(Category = Colors, EditAnywhere, BlueprintReadOnly)
    FLinearColor BaseMaskColor;

    UPROPERTY(Category = Colors, EditAnywhere, BlueprintReadOnly)
    bool bUseOutline = true;

    UPROPERTY(Category = Colors, EditAnywhere, BlueprintReadOnly)
    FLinearColor OutlineColor;

    UPROPERTY(Category = Colors, EditAnywhere, BlueprintReadOnly)
    bool bUseContour = true;

    UPROPERTY(Category = Colors, EditAnywhere, BlueprintReadOnly)
    FLinearColor ContourColor;

    UPROPERTY(Category = Colors, EditAnywhere, BlueprintReadOnly)
	bool bUseGlassMask = true;

	UPROPERTY(Category = Colors, EditAnywhere, BlueprintReadOnly)
	FLinearColor GlassMaskColor;

	UPROPERTY(Category = Colors, EditAnywhere, BlueprintReadOnly)
	EGlassMaskType GlassMaskType;

};
