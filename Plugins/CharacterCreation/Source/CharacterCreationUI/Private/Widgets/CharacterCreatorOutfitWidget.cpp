// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CharacterCreatorOutfitWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Widgets/CharacterCreatorOutfitVariantWidget.h"
#include "CharacterCreatorOutfit.h"
#include "Components/CharacterCreatorControlComponent.h"
#include "Components/HorizontalBoxSlot.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

bool UCharacterCreatorOutfitWidget::Initialize()
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

	SubmitButton->OnPressed.AddDynamic(this, &UCharacterCreatorOutfitWidget::OnButtonPressed);

	RefreshWidget();

	return true;
}

void UCharacterCreatorOutfitWidget::SetOutfit(const UCharacterCreatorOutfit* NewOutfit)
{
	Outfit = NewOutfit;
	RefreshWidget();
}

void UCharacterCreatorOutfitWidget::RefreshWidget()
{
	if (!Outfit)
	{
		return;
	}

	if (SelectedOutfitName)
	{
		SelectedOutfitName->SetText(Outfit->FriendlyName);
	}

	if (IconImage)
	{
		IconImage->SetBrushFromTexture(Outfit->Icon);
	}

	if (VariantPanelWidget) //Optional
	{
		VariantPanelWidget->ClearChildren();

		if (Outfit->MaterialVariants.IsEmpty())
		{
			VariantPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			VariantPanelWidget->SetVisibility(ESlateVisibility::Visible);

			for (int32 i = 0; i < Outfit->MaterialVariants.Num(); i++)
			{
				const FCharacterCreatorMaterialVariant& MaterialVariant = Outfit->MaterialVariants[i];
				UCharacterCreatorOutfitVariantWidget* Widget = CreateWidget<UCharacterCreatorOutfitVariantWidget>(this, OutfitVariantWidgetClass);
				if (Widget)
				{
					Widget->SetValues(MaterialVariant, Outfit, i);
					UPanelSlot* VariantPanelSlot = VariantPanelWidget->AddChild(Widget);
					if (UHorizontalBoxSlot* VerticalSlot = Cast<UHorizontalBoxSlot>(VariantPanelSlot))
					{
						VerticalSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
					}
				}
			}
		}
	}
}

void UCharacterCreatorOutfitWidget::OnButtonPressed()
{
	UCharacterCreatorControlComponent* CCControlComp = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(GetOwningPlayer());
	if (!CCControlComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCOutfitListEntryWidget::OnValueChanged() No UCharacterCreatorControlComponent Found"));
		return;
	}

	CCControlComp->Server_SetOutfit(CCControlComp->MainCharacterCreator, Outfit);
	OnOutfitSelectedEvent.Broadcast();
}

bool UCharacterCreatorOutfitWidget::IsListItemSelectable() const
{
	return true;
}

void UCharacterCreatorOutfitWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	SetOutfit(Cast<UCharacterCreatorOutfit>(ListItemObject));
}
