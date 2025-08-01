// Created by Jorge Puerto. All Rights Reserved.


#include "Engine/JPEGameViewportClient.h"


UJPEGameViewportClient::UJPEGameViewportClient(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UJPEGameViewportClient::Init(FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice)
{
	Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);
}

void UJPEGameViewportClient::Draw(FViewport* InViewport, FCanvas* InSceneCanvas)
{
	if (!bDisableWorldRenderingJPE)
	{
		Super::Draw(InViewport, InSceneCanvas);
	}
}
