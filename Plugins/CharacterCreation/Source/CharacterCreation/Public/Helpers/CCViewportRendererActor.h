// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "CCViewportRendererActor.generated.h"

class USceneCaptureComponent2D;
class UTextureRenderTarget2D;
class FSceneInterface;
class UChildActorComponent;
class UCharacterCreatorComponent;
class UCharacterCreator;
class USkeletalMeshComponent;

UCLASS()
class CHARACTERCREATION_API ACCViewportRendererActor : public ASceneCapture2D
{
	GENERATED_BODY()
	
public:	

	ACCViewportRendererActor();

	void SetRenderTarget(UTextureRenderTarget2D* RenderTarget);
	void SetOpacityRenderTarget(UTextureRenderTarget2D* OpacityRenderTarget);
	void SetCharacterCreator(UCharacterCreator* NewCharacterCreator);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USceneCaptureComponent2D> OpacityCaptureComponent2D;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCharacterCreatorComponent* CharacterCreatorComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;
};
