// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseEditorTool.h"
#include "UObject/NoExportTypes.h"
#include "JPEMirrorTool.generated.h"

/**
 *
 */
UCLASS()
class JPEEDITOR_API UJPEMirrorTool : public UBaseEditorTool
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, Category = "Settings")
	FPlane MirrorPlane;

	UFUNCTION(Exec, Category = "Settings")
	void MirrorSelectedObjects();

};
