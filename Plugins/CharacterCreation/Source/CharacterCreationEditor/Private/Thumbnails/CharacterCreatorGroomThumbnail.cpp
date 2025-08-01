// Created by Jorge Puerto. All Rights Reserved.


#include "Thumbnails/CharacterCreatorGroomThumbnail.h"
#include "CharacterCreatorGroom.h"


bool UCharacterCreatorGroomThumbnail::CanVisualizeAsset(UObject* Object)
{
	UCharacterCreatorGroom* CharacterCreatorGroom = Cast<UCharacterCreatorGroom>(Object);
	if (CharacterCreatorGroom == nullptr)
	{
		return false;
	}
	UTexture2D* Texture = CharacterCreatorGroom->Icon;
	return Texture != nullptr ? UTextureThumbnailRenderer::CanVisualizeAsset(Texture) : false;
}

void UCharacterCreatorGroomThumbnail::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily)
{
	UCharacterCreatorGroom* CharacterCreatorGroom = Cast<UCharacterCreatorGroom>(Object);
	if (CharacterCreatorGroom == nullptr)
	{
		return;
	}
	UTexture2D* Texture = CharacterCreatorGroom->Icon;
	if (Texture != nullptr)
	{
		UTextureThumbnailRenderer::Draw(Texture, X, Y, Width, Height, RenderTarget, Canvas, bAdditionalViewFamily);
	}
}