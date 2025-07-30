// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JPEPortfolioPlayerController.generated.h"

class UInputMappingContext;
class UFocusTracerComponent;
class UInputAction;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class AJPEPortfolioPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Focus")
	FViewTargetTransitionParams DefaultCameraTransitionParams;

	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Focus")
	UFocusTracerComponent* FocusTracerComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FocusAction1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FocusAction2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FocusAction3;

protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> DefaultMappingContexts;

public:
	AJPEPortfolioPlayerController();

	virtual void SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams) override;


protected:

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	virtual void Action1Pressed();
	virtual void Action2Pressed();
	virtual void Action3Pressed();

};
