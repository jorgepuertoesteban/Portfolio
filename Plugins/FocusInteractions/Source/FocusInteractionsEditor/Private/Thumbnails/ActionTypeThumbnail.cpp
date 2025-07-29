// Created by Jorge Puerto. All Rights Reserved.


#include "Thumbnails/ActionTypeThumbnail.h"
#include "ActionType.h"


bool UActionTypeThumbnail::CanVisualizeAsset(UObject* Object)
{
	UActionType* ActionType = Cast<UActionType>(Object);
	if (ActionType == nullptr)
	{
		return false;
	}
	UTexture2D* Texture = ActionType->GetIcon();
	return Texture != nullptr ? UTextureThumbnailRenderer::CanVisualizeAsset(Texture) : false;
}

void UActionTypeThumbnail::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily)
{
	UActionType* ActionType = Cast<UActionType>(Object);
	if (ActionType == nullptr)
	{
		return;
	}
	UTexture2D* Texture = ActionType->GetIcon();
	if (Texture != nullptr)
	{
		UTextureThumbnailRenderer::Draw(Texture, X, Y, Width, Height, RenderTarget, Canvas, bAdditionalViewFamily);
	}
}