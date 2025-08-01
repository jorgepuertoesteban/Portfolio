// Created by Jorge Puerto. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActorActionUtility.h"
#include "Materials/MaterialInterface.h"
#include "MyActorActionUtility.generated.h"

/**
 * 
 */
UCLASS()
class JPEEDITOR_API UMyActorActionUtility : public UActorActionUtility
{
	GENERATED_BODY()
	
public:

	UFUNCTION(CallInEditor)
	void ChangeMaterial(UMaterialInterface* NewMaterial);

};
