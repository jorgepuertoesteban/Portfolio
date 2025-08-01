// Created by Jorge Puerto. All Rights Reserved.


#include "Widgets/CharacterCreatorV2Widget.h"
#include "Components/CharacterCreatorControlComponent.h"
#include "Components/Button.h"

bool UCharacterCreatorV2Widget::Initialize()
{
	if (!Super::Initialize())
	{
		//TODO: LOG	
		return false;
	}

	if (!CharacterCreatorModel)
	{
		//TODO: LOG
		return false;
	}

	if (!SaveButton)
	{
		//TODO: LOG
		return false;
	}

	SaveButton->OnPressed.AddDynamic(this, &UCharacterCreatorV2Widget::OnSaveButtonPressed);

	return true;
}

void UCharacterCreatorV2Widget::OnSaveButtonPressed()
{
	UCharacterCreatorControlComponent* CCControlComp = UCharacterCreatorControlComponent::GetCharacterCreatorControlComponent(GetOwningPlayer());
	if (!CCControlComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCOutfitListEntryWidget::OnValueChanged() No UCharacterCreatorControlComponent Found"));
		return;
	}
	CCControlComp->Server_SaveCharacterCreator();
}

