// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "JPEUISubsystem.generated.h"

class UJPEUIConfig;
class UAlertWidget;
class UWidget;
class UConfirmWidget;
class OwnerTy;
class UUserWidget;
class APlayerController;
class AHUD;

/**
 *
 */
UCLASS(Config = JPE)
class JPEUI_API UJPEUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:

	static UJPEUISubsystem* MyInstance;

	UPROPERTY()
	UJPEUIConfig* UIConfig;

private:

	UPROPERTY(Config)
	bool bShouldDisableWorldRendering = false;

	UPROPERTY(Config)
	TSoftObjectPtr<UJPEUIConfig> UIConfigProxy;

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	static UJPEUISubsystem* GetInstance() { return MyInstance; }

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	template <typename OwnerType = UObject>
	static UAlertWidget* CreateAlertWidget(OwnerType* Owner, const FText& Text, const int32 ZOrder = 0);
	UFUNCTION(BlueprintCallable)
	static UAlertWidget* CreateAlert(UUserWidget* Owner, const FText& Text, const int32 ZOrder = 0);
	static UAlertWidget* CreateAlert(APlayerController* Owner, const FText& Text, const int32 ZOrder = 0);

	template <typename OwnerType = UObject>
	static UConfirmWidget* CreateConfirmWidget(OwnerType* Owner, const FText& Text, const int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable)
	static UConfirmWidget* CreateConfirm(UUserWidget* Owner, const FText& Text, const int32 ZOrder = 0);

	static UConfirmWidget* CreateConfirm(APlayerController* Owner, const FText& Text, const int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable)
	UJPEUIConfig* GetUIConfig();
};