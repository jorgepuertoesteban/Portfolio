// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CharacterCreatorOutfitsSetWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "CharacterCreatorOutfitsSet.h"
#include "Widgets/CharacterCreatorOutfitWidget.h"
#include "Components/PanelWidget.h"

bool UCharacterCreatorOutfitsSetWidget::Initialize()
{
	if (!Super::Initialize())
	{
		UE_LOG(LogTemp, Error, TEXT("UCategoryTabContentWidget::Initialize() Super::Initialize failed"));
		return false;
	}

	if (!PanelWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("HorizontalBox is null"));
		return false;
	}

	RefreshWidget();

	return true;
}

void UCharacterCreatorOutfitsSetWidget::SetOutfitsSet(const UCharacterCreatorOutfitsSet* NewOutfitsSet)
{
	OutfitsSet = NewOutfitsSet;
	RefreshWidget();
}

void UCharacterCreatorOutfitsSetWidget::RefreshWidget()
{
	if (!OutfitsSet)
	{
		return;
	}

	for (const UCharacterCreatorOutfit* Outfit : OutfitsSet->Outfits)
	{
		if (UCharacterCreatorOutfitWidget* Widget = CreateWidget<UCharacterCreatorOutfitWidget>(this, OutfitWidgetClass))
		{
			Widget->SetOutfit(Outfit);
			PanelWidget->AddChild(Widget);
			Widget->OnOutfitSelectedEvent.AddDynamic(this, &UCharacterCreatorOutfitsSetWidget::OutfitSelected);
		}
	}
}