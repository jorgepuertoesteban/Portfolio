// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "JPEGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class JPECORE_API UJPEGameViewportClient : public UGameViewportClient
{
	GENERATED_UCLASS_BODY()
protected:
	bool bDisableWorldRenderingJPE = true;

public:
	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;

	virtual void Draw(FViewport* InViewport, FCanvas* InSceneCanvas) override;

	void SetJPEDisableWorldRendering(const bool bNewValue) { bDisableWorldRenderingJPE = bNewValue; }
};
