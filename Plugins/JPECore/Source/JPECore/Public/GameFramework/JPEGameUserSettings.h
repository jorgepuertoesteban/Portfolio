// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Structs/JPEInputSettings.h"
#include "Structs/MappableActionTraditional.h"
#include "Utils/JPEInputUtils.h"
#include "JPEGameUserSettings.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJPEGameUserSettingsLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJPEGameUserSettingsApplied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJPEGameUserSettingsSetToDefaults);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQualitySettingsEdited);

UCLASS(Config = JPE)
class JPECORE_API UJPEGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnJPEGameUserSettingsLoaded OnSettingsLoaded;
	UPROPERTY(BlueprintAssignable)
	FOnJPEGameUserSettingsApplied OnSettingsApplied;
	UPROPERTY(BlueprintAssignable)
	FOnJPEGameUserSettingsSetToDefaults OnSettingsSetToDefaults;
	UPROPERTY(BlueprintAssignable)
	FOnQualitySettingsEdited OnQualitySettingsEdited;

protected:

	UPROPERTY(Config)
	TArray<FJPEInputSettings> DefaultInputSettings;

	UPROPERTY(Transient)
	UJPEGameUserSettings* BackupSettings;
	UPROPERTY(Transient)
	bool bIsResettingToDefaults = false;

	UPROPERTY()
	float Gamma;
	UPROPERTY()
	FString Language;

	UPROPERTY()
	TMap<FMappableActionTraditional,FKey> InputKeysByActionTraditional;
	UPROPERTY()
	TMap<FMappableActionTraditional,float> InputSensitivitiesByActionTraditional;
	UPROPERTY()
	TMap<FMappableActionTraditional,bool> InputInvertsByActionTraditional;
	UPROPERTY()
	TMap<FMappableActionTraditional,float> InputDeadZonesByActionTraditional;

	UPROPERTY()
	TMap<FMappableActionEnhanced,FKey> InputKeysByActionEnhanced;
	UPROPERTY()
	TMap<FMappableActionEnhanced,float> InputSensitivitiesByActionEnhanced;
	UPROPERTY()
	TMap<FMappableActionEnhanced,bool> InputInvertsByActionEnhanced;
	UPROPERTY()
	TMap<FMappableActionEnhanced,float> InputDeadZonesByActionEnhanced;

public:

	virtual void LoadSettings(bool bForceReload = false) override;
	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
	virtual void SetToDefaults() override;

	UFUNCTION(BlueprintCallable)
	static UJPEGameUserSettings* GetJPEGameUserSettings();

	void ApplySettingsSafe();
	void RevertAppliedSettings();
	void KeepAppliedSettings();
	void ResetToDefaults();

#pragma region Getters&Setters
	UFUNCTION(BlueprintCallable)
	bool IsResettingToDefaults() const { return bIsResettingToDefaults; }

	UFUNCTION(BlueprintCallable)
	float GetGamma() const { return Gamma; }
	UFUNCTION(BlueprintCallable)
	void SetGamma(const float NewGamma) { Gamma = NewGamma; }

	UFUNCTION(BlueprintCallable)
	FString GetLanguage() const { return Language; }
	UFUNCTION(BlueprintCallable)
	void SetLanguage(const FString& NewLanguage) { Language = NewLanguage; }

	template<typename FunctorType>
	void GetDefaultInputSettingsWithCallback(const FName& ActionName, FunctorType&& Callback);

	UFUNCTION(BlueprintCallable)
	FMappableActionTraditional GetActionForKeyTraditional(const FKey& Key) const;
	UFUNCTION(BlueprintCallable)
	FKey GetInputKeyForActionTraditional(const FMappableActionTraditional& Action);
	UFUNCTION(BlueprintCallable)
	void SetInputKeyForActionTraditional(const FMappableActionTraditional& Action, const FKey& NewInputKey);

	UFUNCTION(BlueprintCallable)
	float GetInputSensitivityTraditional(const FMappableActionTraditional& Action);
	UFUNCTION(BlueprintCallable)
	void SetInputSensitivityTraditional(const FMappableActionTraditional& Action, const float NewSensitivity);

	UFUNCTION(BlueprintCallable)
	bool GetInputInvertTraditional(const FMappableActionTraditional& Action);
	UFUNCTION(BlueprintCallable)
	void SetInputInvertTraditional(const FMappableActionTraditional& Action, const bool bNewInvert);

	UFUNCTION(BlueprintCallable)
	float GetInputDeadZoneTraditional(const FMappableActionTraditional& Action);
	UFUNCTION(BlueprintCallable)
	void SetInputDeadZoneTraditional(const FMappableActionTraditional& Action, const float NewDeadZone);

	UFUNCTION(BlueprintCallable)
	FMappableActionEnhanced GetActionForKeyEnhanced(const FKey& Key) const;
	UFUNCTION(BlueprintCallable)
	FKey GetInputKeyForActionEnhanced(const FMappableActionEnhanced& Action);
	UFUNCTION(BlueprintCallable)
	void SetInputKeyForActionEnhanced(const FMappableActionEnhanced& Action, const FKey& NewKey);

	UFUNCTION(BlueprintCallable)
	float GetInputSensitivityEnhanced(const FMappableActionEnhanced& Action);
	UFUNCTION(BlueprintCallable)
	void SetInputSensitivityEnhanced(const FMappableActionEnhanced& Action, const float NewSensitivity);

	UFUNCTION(BlueprintCallable)
	bool GetInputInvertEnhanced(const FMappableActionEnhanced& Action);
	UFUNCTION(BlueprintCallable)
	void SetInputInvertEnhanced(const FMappableActionEnhanced& Action, const bool bNewInvert);

	UFUNCTION(BlueprintCallable)
	float GetInputDeadZoneEnhanced(const FMappableActionEnhanced& Action);
	UFUNCTION(BlueprintCallable)
	void SetInputDeadZoneEnhanced(const FMappableActionEnhanced& Action, const float NewDeadZone);
#pragma endregion

};
