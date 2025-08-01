// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseEditorTool.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class JPEEDITOR_API UBaseEditorTool : public UObject
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere,Category="Hidden")
	FText HelpText;
};
