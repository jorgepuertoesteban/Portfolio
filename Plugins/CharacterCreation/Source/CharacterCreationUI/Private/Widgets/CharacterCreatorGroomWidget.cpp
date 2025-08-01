// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CharacterCreatorGroomWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "CharacterCreatorGroom.h"
#include "Components/CharacterCreatorControlComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"


bool UCharacterCreatorGroomWidget::Initialize()
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

	SubmitButton->OnPressed.AddDynamic(this, &UCharacterCreatorGroomWidget::OnButtonPressed);

	RefreshWidget();

	return true;
}

void UCharacterCreatorGroomWidget::SetGroom(const UCharacterCreatorGroom* NewGroom)
{
	Groom = NewGroom;
	RefreshWidget();
}

void UCharacterCreatorGroomWidget::RefreshWidget()
{
	if (!Groom)
	{
		return;
	}

	if (SelectedGroomName)
	{
		SelectedGroomName->SetText(Groom->FriendlyName);
	}

	if (IconImage)
	{
		IconImage->SetBrushFromTexture(Groom->Icon);
	}

}

void UCharacterCreatorGroomWidget::OnButtonPressed()
{
	UCharacterCreatorControlComponent* CCControlComp = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(GetOwningPlayer());
	if (!CCControlComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCGroomListEntryWidget::OnValueChanged() No UCharacterCreatorControlComponent Found"));
		return;
	}

	CCControlComp->Server_SetGroom(CCControlComp->MainCharacterCreator, Groom);
	OnGroomSelectedEvent.Broadcast();
}

bool UCharacterCreatorGroomWidget::IsListItemSelectable() const
{
	return true;
}

void UCharacterCreatorGroomWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	SetGroom(Cast<UCharacterCreatorGroom>(ListItemObject));
}
