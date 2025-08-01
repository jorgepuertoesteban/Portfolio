// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterCreatorHUDComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CHARACTERCREATION_API UCharacterCreatorHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "CharacterCreation")
	TSubclassOf<UUserWidget> CharacterCreatorWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "CharacterCreation")
	UUserWidget* CharacterCreatorWidget;

public:
	UFUNCTION(BlueprintCallable)
	static UCharacterCreatorHUDComponent* GetCharacterCreatorHUDComponent(AActor* OwnerActor);

	UFUNCTION(BlueprintCallable)
	void ShowCharacterCreator();

	UFUNCTION(BlueprintCallable)
	void HideCharacterCreator() const;

};
