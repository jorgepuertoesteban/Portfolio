// Created by Jorge Puerto. All Rights Reserved.


#include "Thumbnails/CharacterCreatorOutfitThumbnail.h"
#include "Engine/Texture2D.h"
#include "CharacterCreatorOutfit.h"


bool UCharacterCreatorOutfitThumbnail::CanVisualizeAsset(UObject* Object)
{
	UCharacterCreatorOutfit* CharacterCreatorOutfit = Cast<UCharacterCreatorOutfit>(Object);
	if (CharacterCreatorOutfit == nullptr)
	{
		return false;
	}
	UTexture2D* Texture = CharacterCreatorOutfit->Icon;
	return Texture != nullptr ? UTextureThumbnailRenderer::CanVisualizeAsset(Texture) : false;
}

void UCharacterCreatorOutfitThumbnail::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily)
{
	UCharacterCreatorOutfit* CharacterCreatorOutfit = Cast<UCharacterCreatorOutfit>(Object);
	if (CharacterCreatorOutfit == nullptr)
	{
		return;
	}
	UTexture2D* Texture = CharacterCreatorOutfit->Icon;
	if (Texture != nullptr)
	{
		UTextureThumbnailRenderer::Draw(Texture, X, Y, Width, Height, RenderTarget, Canvas, bAdditionalViewFamily);
	}
}