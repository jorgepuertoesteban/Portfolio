// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EntryPointWidgetManagerComponent.generated.h"


class UWidget;
class UUserWidget;
class APawn;
class APlayerController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JPEUI_API UEntryPointWidgetManagerComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<TSubclassOf<UUserWidget>> EntryClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<TSubclassOf<UUserWidget>> EntryClassesToStack;

	UPROPERTY(Transient)
	UWidget* EntryPointWidget;

	UPROPERTY(Transient)
	TArray<UWidget*> EntryPointWidgetStack;

protected:
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	virtual void OnRegister() override;
	virtual void BeginPlay() override;

public:
	virtual bool IsAnyEntryPointActive() const
	{
		return EntryPointWidget != nullptr;
	}

	APlayerController* GetOwningPlayerController() const;

	APawn* GetOwningPawn() const;

	void ResetDesiredInput() const;

	static UEntryPointWidgetManagerComponent* GetEntryPointWidgetManagerComponent(const AActor* Owner);
};
