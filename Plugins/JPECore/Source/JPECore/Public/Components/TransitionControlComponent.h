// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TransitionControlComponent.generated.h"

DECLARE_DELEGATE(FFadeCompletedDelegate);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JPECORE_API UTransitionControlComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	TMap<const uint32, const FFadeCompletedDelegate> DelegatesMap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FadeDuration = .3f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AckDelay = .0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float InitialDelay = .0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BeginPlayInitialDelay = 2.f;

	UPROPERTY(EditDefaultsOnly)
	bool FadeOutOnBeginPlay = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	FLinearColor FadeColor = FLinearColor::Black;

public:
	UTransitionControlComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void BeginDestroy() override;
	
	static uint32 NextDelegateKey()
	{
		static uint32 NextKey = 0;
		return NextKey++;
	}

	APlayerController* GetPlayerController() const;

	virtual void SendClientFadeAck(const bool bIsFadeIn, const FFadeCompletedDelegate& FadeCompletedDelegate);

	virtual void SendClientFadeInAck(const FFadeCompletedDelegate& FadeCompletedDelegate);

	virtual void SendClientFadeOutAck(const FFadeCompletedDelegate& FadeCompletedDelegate);

	void LocalFadeOut() const;
	
	virtual void SendClientFadeIn();
	
	virtual void SendClientFadeOut();
	
	virtual void LocalFadeAck(const bool bIsFadeIn, const FFadeCompletedDelegate& FadeCompletedDelegate);

	virtual void LocalFadeInAck(const FFadeCompletedDelegate& FadeCompletedDelegate);

	virtual void LocalFadeOutAck(const FFadeCompletedDelegate& FadeCompletedDelegate);

	void LocalFadeIn() const;

protected:
	virtual void FadeAck(const bool bIsFadeIn, const int32 DelegateKey, const bool bIsLocal);

	UFUNCTION(Client, Reliable, Category="JPECore")
	virtual void Client_Fade(const bool bIsFadeIn);
	
	void StartCameraFade(bool bIsFadeIn) const;

	UFUNCTION(Client, Reliable, Category="JPECore")
	virtual void Client_FadeAck(const bool bIsFadeIn, const int32 DelegateKey);

	UFUNCTION(Server, Reliable, Category="JPECore")
	virtual void Server_NotifyFadeCompleted(const int32 DelegateKey);

	virtual void NotifyFadeCompleted(const int32 DelegateKey);

	void CreateFade(const bool bIsFadeIn)const;

};
