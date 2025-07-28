// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JPEPortfolioPlayerController.generated.h"

class UInputMappingContext;
class UFocusTracerComponent;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class AJPEPortfolioPlayerController : public APlayerController
{
	GENERATED_BODY()

	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Focus")
	UFocusTracerComponent* FocusTracerComponent;

protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> DefaultMappingContexts;

public:
	AJPEPortfolioPlayerController();

protected:

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

};
