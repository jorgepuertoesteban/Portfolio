// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PointAndClickHUDComponent.generated.h"

class UPointAndClickMenuWidget;
class APlayerController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FOCUSINTERACTIONS_API UPointAndClickHUDComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FFooDelegate, int32);

protected:
	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TSubclassOf<UPointAndClickMenuWidget> PointAndClickMenuWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Widgets)
	UPointAndClickMenuWidget* PointAndClickMenuWidget;

public:
	static UPointAndClickHUDComponent* GetPointAndClickHUDComponent(const AHUD* HUD);
	
	APlayerController* GetOwningPlayerController() const;

	void ClearWidgets();

	void ShowInGameWidget();

	void SelectAction(int32 ActionIndex) const;
};
