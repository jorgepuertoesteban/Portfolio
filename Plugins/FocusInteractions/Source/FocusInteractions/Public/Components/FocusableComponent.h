// Created by Bionic Ape. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FocusInteractionsTypes.h"
#include "FocusableComponent.generated.h"

class UMeshComponent;
class USceneComponent;
class UActionableType;
class UActionType;
class UFocusComponent;
class AController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestExecuteAction, const FFocusActionParams&, Params);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartFocusDelegate, const FFocusInfo, FocusTraceInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndFocusDelegate);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FOCUSINTERACTIONS_API UFocusableComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	int32 StencilFocusValue = 255;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<UMeshComponent>> Meshes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions", Replicated)
	UActionableType* ActionableType;

	UPROPERTY(BlueprintAssignable, Category = "Actions")
	FOnRequestExecuteAction OnRequestExecuteAction;

	UPROPERTY(BlueprintAssignable, Category = "Actions")
	FOnStartFocusDelegate OnStartFocusDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Actions")
	FOnEndFocusDelegate OnEndFocusDelegate;

	UFocusableComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UFocusableComponent* GetFocusableComponent(AActor* Actor);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "Set this Component to Focus Mode"), Category = "Actions")
	void StartFocus(UFocusComponent const* const FocusComponent);

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "Unset this Component to Focus Mode"), Category = "Actions")
	void EndFocus(UFocusComponent const* const FocusComponent);

	void SetRenderCustomDepth();

	UFUNCTION(BlueprintCallable, Category = Switches)
	virtual void ExecuteAction(const FFocusActionParams& Params);

};
