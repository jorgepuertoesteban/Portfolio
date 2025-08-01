// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ThumbnailRendering/TextureThumbnailRenderer.h"
#include "CharacterCreatorGroomThumbnail.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERCREATIONEDITOR_API UCharacterCreatorGroomThumbnail : public UTextureThumbnailRenderer
{
	GENERATED_BODY()

	//~ Begin UThumbnailRenderer Interface.
	virtual bool CanVisualizeAsset(UObject* Object) override;
	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget*, FCanvas* Canvas, bool bAdditionalViewFamily) override;
	//~ EndUThumbnailRenderer Interface.
};
