// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CharacterCreatorOutfitVariantWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "CharacterCreatorOutfit.h"
#include "Components/Button.h"
#include "Components/CharacterCreatorControlComponent.h"


bool UCharacterCreatorOutfitVariantWidget::Initialize()
{
	if (!Super::Initialize())
	{
		UE_LOG(LogTemp, Error, TEXT("Initialize() Super::Initialize failed"));
		return false;
	}

	if (!SubmitButton)
	{
		UE_LOG(LogTemp, Error, TEXT("SubmitButton is null"));
		return false;
	}
	
	SubmitButton->OnPressed.AddDynamic(this, &UCharacterCreatorOutfitVariantWidget::OnButtonPressed);

	RefreshWidget();

	return true;
}

void UCharacterCreatorOutfitVariantWidget::SetValues(const FCharacterCreatorMaterialVariant& NewOutfitVariant, const UCharacterCreatorOutfit* NewOutfit, const int32 NewIndex)
{
	OutfitVariant = NewOutfitVariant;
	Outfit = NewOutfit;
	VariantIndex = NewIndex;
	RefreshWidget();
}


void UCharacterCreatorOutfitVariantWidget::RefreshWidget()
{
	if (SelectedOutfitName)
	{
		SelectedOutfitName->SetText(OutfitVariant.DisplayName);
	}

	if (IconImage)
	{
		IconImage->SetColorAndOpacity(OutfitVariant.Thumbnail);
	}
	
	FLinearColor NormalColor = OutfitVariant.Thumbnail;
	NormalColor.A = 1.f;
	FLinearColor HoveredColor = OutfitVariant.Thumbnail;
	HoveredColor.A = 0.5;
	
	FButtonStyle Style = SubmitButton->GetStyle();
	Style.Normal.TintColor = NormalColor;
	Style.Hovered.TintColor = HoveredColor;
	SubmitButton->SetStyle(Style);

	SetToolTipText(OutfitVariant.DisplayName);
}

void UCharacterCreatorOutfitVariantWidget::OnButtonPressed()
{
	UCharacterCreatorControlComponent* CCControlComp = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(GetOwningPlayer());
	if (!CCControlComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCOutfitListEntryWidget::OnValueChanged() No UCharacterCreatorControlComponent Found"));
		return;
	}

	CCControlComp->Server_SetOutfit(CCControlComp->MainCharacterCreator, Outfit, VariantIndex);
}