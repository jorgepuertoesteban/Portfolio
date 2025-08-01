// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CharacterCreatorRendererWidget.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "CanvasTypes.h"
#include "EngineModule.h"
#include "Components/Viewport.h"

UCharacterCreatorRendererWidget::UCharacterCreatorRendererWidget(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RenderTargetFinder(TEXT("/Script/Engine.TextureRenderTarget2D'/CharacterCreation/UI/RenderTargets/CharacterRenderTarget.CharacterRenderTarget'"));
	RenderTargetTexture = RenderTargetFinder.Object;
	check(RenderTargetTexture)
		static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> OpacityRenderTargetFinder(TEXT("/Script/Engine.TextureRenderTarget2D'/CharacterCreation/UI/RenderTargets/OpacityRenderTarget.OpacityRenderTarget'"));
	OpacityRenderTargetTexture = OpacityRenderTargetFinder.Object;
	check(OpacityRenderTargetTexture)
	
}

void UCharacterCreatorRendererWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (!ViewportPanel || !ViewportImage)
	{
		return;
	}

	Viewport = NewObject<UViewport>(UViewport::StaticClass());

	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(ViewportPanel->AddChild(Viewport));
	FAnchors Anchors;
	PanelSlot->SetAnchors(Anchors);

	Renderer = Cast<ACCViewportRendererActor>(Viewport->Spawn(RendererClass));
	Renderer->SetRenderTarget(RenderTargetTexture);
	Renderer->SetOpacityRenderTarget(OpacityRenderTargetTexture);

	if (CharacterCreator)
	{
		Renderer->SetCharacterCreator(CharacterCreator);
	}

	OnConstructedDelegate.ExecuteIfBound();
}

void UCharacterCreatorRendererWidget::SetNewCharacterCreator(UCharacterCreator* NewCharacterCreator)
{
	OnNewCharacterCreator(NewCharacterCreator);

	CharacterCreator = NewCharacterCreator;

	if (!Renderer)
	{
		return;
	}

	Renderer->SetCharacterCreator(CharacterCreator);
}
