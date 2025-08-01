// Created by Jorge Puerto. All rights reserved.


#include "Widgets/CCGroomListEntryWidget.h"
#include "CharacterCreatorGroom.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "DataHelper/CharacterCreatorGroomDH.h"
#include "Components/Button.h"
#include "CharacterCreator.h"
#include "CharacterCreatorGroomsSet.h"
#include "Components/CharacterCreatorControlComponent.h"

bool UCCGroomListEntryWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (LeftButton && RightButton)
		{
			LeftButton->OnClicked.AddDynamic(this, &UCCGroomListEntryWidget::OnLeftClicked);
			RightButton->OnClicked.AddDynamic(this, &UCCGroomListEntryWidget::OnRightClicked);
			return true;
		}
	}
	return false;
}

void UCCGroomListEntryWidget::OnLeftClicked()
{
	SetGroom(true/*Left*/);
}

void UCCGroomListEntryWidget::OnRightClicked()
{
	SetGroom(false/*Right*/);
}


void UCCGroomListEntryWidget::SetGroom(bool bDirectionIsLeft)
{
	if (!CCGroomDH || !CCGroomDH->GroomsSet || !CCGroomDH->CharacterCreator)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCGroomListEntryWidget::OnValueChanged() Bad values !CCGroomDH || !CCGroomDH->GroomsSet || !CCGroomDH->CharacterCreator"));
		return;
	}

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCGroomListEntryWidget::OnValueChanged() No Owning PlayerController Found"));
		return;
	}

	UCharacterCreatorControlComponent* CCControlComp = Cast<UCharacterCreatorControlComponent>(PC->GetComponentByClass(UCharacterCreatorControlComponent::StaticClass()));
	if (!CCControlComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCGroomListEntryWidget::OnValueChanged() No UCharacterCreatorControlComponent Found"));
		return;
	}

	CCGroomDH->SelectedGroom = bDirectionIsLeft ? CCGroomDH->GroomsSet->GetPrevGroom(CCGroomDH->SelectedGroom) : CCGroomDH->GroomsSet->GetNextGroom(CCGroomDH->SelectedGroom);
	CCControlComp->SetGroom(CCGroomDH->CharacterCreator, CCGroomDH->SelectedGroom);
	UpdateGroomText();
}

void UCCGroomListEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	CCGroomDH = Cast<UCharacterCreatorGroomDH>(ListItemObject);

	if (!CCGroomDH && CCGroomDH->CharacterCreator)
	{
		UE_LOG(LogTemp, Error, TEXT("UCCGroomListEntryWidget::NativeOnListItemObjectSet() ListItemObject empty or not of class UCharacterCreatorGroomDH"));
		return;
	}

	UpdateGroomText();

	if (NameTextWidget && CCGroomDH->GroomsSet)
	{
		NameTextWidget->SetText(FText::FromName(CCGroomDH->GroomsSet->FriendlyName));;
	}
}

void UCCGroomListEntryWidget::UpdateGroomText()
{
	if (SelectedGroomName && CCGroomDH->SelectedGroom)
	{
		SelectedGroomName->SetText(CCGroomDH->SelectedGroom->FriendlyName);
	}
}