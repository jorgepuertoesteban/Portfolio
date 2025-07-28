// Created by Bionic Ape. All Rights Reserved.


#include "Thumbnails/ActionsSetThumbnail.h"
#include "ActionsSet.h"

bool UActionsSetThumbnail::CanVisualizeAsset(UObject* Object)
{
	UActionsSet* ActionsSet = Cast<UActionsSet>(Object);
	if (ActionsSet == nullptr)
	{
		return false;
	}
	UTexture2D* Texture = ActionsSet->GetIcon();
	return Texture != nullptr ? UTextureThumbnailRenderer::CanVisualizeAsset(Texture) : false;
}

void UActionsSetThumbnail::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily)
{
	UActionsSet* ActionsSet = Cast<UActionsSet>(Object);
	if (ActionsSet == nullptr)
	{
		return;
	}
	UTexture2D* Texture = ActionsSet->GetIcon();
	if (Texture != nullptr)
	{
		UTextureThumbnailRenderer::Draw(Texture, X, Y, Width, Height, RenderTarget, Canvas, bAdditionalViewFamily);
	}
}