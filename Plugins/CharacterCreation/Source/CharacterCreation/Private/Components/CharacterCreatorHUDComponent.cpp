// Created by Jorge Puerto. All Rights Reserved.


#include "Components/CharacterCreatorHUDComponent.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"

UCharacterCreatorHUDComponent* UCharacterCreatorHUDComponent::GetCharacterCreatorHUDComponent(AActor* OwnerActor)
{
	return OwnerActor ? OwnerActor->FindComponentByClass<UCharacterCreatorHUDComponent>() : nullptr;
}

void UCharacterCreatorHUDComponent::ShowCharacterCreator()
{
#pragma region Checks
	if (CharacterCreatorWidget && CharacterCreatorWidget->IsInViewport())
	{
		UE_LOG(LogTemp, Warning, TEXT("UCharacterCreatorHUDComponent::ShowCharacterCreator(): Character creator is already visible."));
		return;
	}

	APlayerController* PC = Cast<AHUD>(GetOwner())->GetOwningPlayerController();
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCharacterCreatorHUDComponent::ShowCharacterCreator(): CharacterCreatorHUDComp has no valid PlayerController."));
		return;
	}
#pragma endregion

	if (!CharacterCreatorWidget)
	{
		if (!CharacterCreatorWidgetClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("UAchievementHUDComponent::ShowCharacterCreator(const UAchievements*): AchievementHUDComp has no valid CharacterCreatorWidgetClass."));
			return;
		}
		CharacterCreatorWidget = CreateWidget<UUserWidget>(PC, CharacterCreatorWidgetClass);
	}

	CharacterCreatorWidget->AddToViewport();
}

void UCharacterCreatorHUDComponent::HideCharacterCreator() const
{
#pragma region Checks
	if (!CharacterCreatorWidget || !CharacterCreatorWidget->IsInViewport())
	{
		UE_LOG(LogTemp, Warning, TEXT("UCharacterCreatorHUDComponent::HideCharacterCreator(): Character creator is already hidden."));
		return;
	}
	APlayerController* PC = Cast<AHUD>(GetOwner())->GetOwningPlayerController();
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCharacterCreatorHUDComponent::HideCharacterCreator(): Character creator has no valid PlayerController."));
		return;
	}
#pragma endregion
	CharacterCreatorWidget->RemoveFromParent();
}
