// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Helpers/CCViewportRendererActor.h"
#include "CharacterCreatorRendererWidget.generated.h"

class UTextureRenderTarget2D;
class UCharacterCreator;
class UViewport;
class UPanelWidget;
class UImage;

DECLARE_DELEGATE(FOnCharacterCreatorRendererWidgetConstruct);


UCLASS()
class CHARACTERCREATIONUI_API UCharacterCreatorRendererWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()
	
public:

	FOnCharacterCreatorRendererWidgetConstruct OnConstructedDelegate;

	UFUNCTION(BlueprintImplementableEvent, Category = "Viewport")
	void OnNewCharacterCreator(UCharacterCreator* NewCharacterCreator);

	virtual void SetNewCharacterCreator(UCharacterCreator* NewCharacterCreator);

	ACCViewportRendererActor* GetRenderer(){return Renderer;}

protected:

	virtual void NativeConstruct() override;

protected:
	
	UPROPERTY(meta=(BindWidget))
	UPanelWidget* ViewportPanel;

	UPROPERTY(meta = (BindWidget))
	UImage* ViewportImage;

	UPROPERTY()
	UTextureRenderTarget2D* RenderTargetTexture;

	UPROPERTY()
	UTextureRenderTarget2D* OpacityRenderTargetTexture;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACCViewportRendererActor> RendererClass = ACCViewportRendererActor::StaticClass();

	UPROPERTY()
	UViewport* Viewport;

	UPROPERTY()
	ACCViewportRendererActor* Renderer;

	UPROPERTY()
	UCharacterCreator* CharacterCreator = nullptr;
};

