// Created by Jorge Puerto. All rights reserved.


#include "Widgets/CCOutfitListEntryWidget.h"
#include "CharacterCreatorOutfit.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "DataHelper/CharacterCreatorOutfitDH.h"
#include "Components/Button.h"
#include "CharacterCreator.h"
#include "CharacterCreatorOutfitsSet.h"
#include "Components/CharacterCreatorControlComponent.h"
#include "Components/Image.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

bool UCCOutfitListEntryWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (LeftButton && RightButton)
		{
			LeftButton->OnClicked.AddDynamic(this, &UCCOutfitListEntryWidget::OnLeftClicked);
			RightButton->OnClicked.AddDynamic(this, &UCCOutfitListEntryWidget::OnRightClicked);
			return true;
		}
	}
	return false;
}

void UCCOutfitListEntryWidget::OnLeftClicked()
{
	SetOutfit(true/*Left*/);
}

void UCCOutfitListEntryWidget::OnRightClicked()
{
	SetOutfit(false/*Right*/);
}


void UCCOutfitListEntryWidget::SetOutfit(bool bDirectionIsLeft)
{
	if (!CCOutfitDH || !CCOutfitDH->OutfitsSet || !CCOutfitDH->CharacterCreator)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCOutfitListEntryWidget::OnValueChanged() Bad values !CCOutfitDH || !CCOutfitDH->OutfitsSet || !CCOutfitDH->CharacterCreator"));
		return;
	}

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCOutfitListEntryWidget::OnValueChanged() No Owning PlayerController Found"));
		return;
	}

	UCharacterCreatorControlComponent* CCControlComp = Cast<UCharacterCreatorControlComponent>(PC->GetComponentByClass(UCharacterCreatorControlComponent::StaticClass()));
	if (!CCControlComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCOutfitListEntryWidget::OnValueChanged() No UCharacterCreatorControlComponent Found"));
		return;
	}

	CCOutfitDH->SelectedOutfit = bDirectionIsLeft ? CCOutfitDH->OutfitsSet->GetPrevOutfit(CCOutfitDH->SelectedOutfit) : CCOutfitDH->OutfitsSet->GetNextOutfit(CCOutfitDH->SelectedOutfit);
	CCControlComp->SetOutfit(CCOutfitDH->CharacterCreator, CCOutfitDH->SelectedOutfit);
	RefreshWidget();
}

void UCCOutfitListEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	CCOutfitDH = Cast<UCharacterCreatorOutfitDH>(ListItemObject);

	if (!CCOutfitDH && CCOutfitDH->CharacterCreator)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCOutfitListEntryWidget::NativeOnListItemObjectSet() ListItemObject empty or not of class UCharacterCreatorOutfitDH"));
		return;
	}

	RefreshWidget();

	if (NameTextWidget && CCOutfitDH->OutfitsSet)
	{
		NameTextWidget->SetText(CCOutfitDH->OutfitsSet->FriendlyName);
	}
}

void UCCOutfitListEntryWidget::RefreshWidget()
{
	if (SelectedOutfitName && CCOutfitDH->SelectedOutfit)
	{
		SelectedOutfitName->SetText(CCOutfitDH->SelectedOutfit->FriendlyName);
	}

	if (IconImage)
	{
		IconImage->SetBrushFromTexture(CCOutfitDH->SelectedOutfit->Icon);
	}
}