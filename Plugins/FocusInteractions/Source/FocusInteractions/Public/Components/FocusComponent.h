 //Created by Bionic Ape. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "FocusInteractionsTypes.h"
#include "FocusComponent.generated.h"

class AController;
class UActionsSet;
class UActionType; 
class UFocusableComponent;
class UFocusableFilter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewFocus, const FFocusInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndFocus, const UFocusableComponent*, Focusable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewActionsSets);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFocusActionPerformed, const FFocusPerformAction&);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FOCUSINTERACTIONS_API UFocusComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(ReplicatedUsing="OnRep_ActionsSets", EditAnywhere, BlueprintReadWrite, Category = "Focus")
	TArray<UActionsSet*> ActionsSets;

	UPROPERTY(Transient)
	AController* OwnerController;
	
	UPROPERTY(Transient)
	APawn* OwnerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
	TArray<const UFocusableFilter*> Filters;

public:

	UPROPERTY(BlueprintReadOnly, /*Transient, */Category = "Focus")
	FFocusInfo FocusInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
	bool bIsThirdPerson = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
	bool bAutoDiscoverOwner = true;

	/**
	 * bForceCloseDistance is usefull to allow Actions that are farther from the defined MaxDistance in the ActionType. Is set to true in UFocusCursorComponent.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus")
	bool bForceCloseDistance = false;

	UPROPERTY(BlueprintAssignable, Category = "Focus")
	FOnNewFocus OnNewFocus;

	UPROPERTY(BlueprintAssignable, Category = "Focus")
	FOnEndFocus OnEndFocus;
	
	UPROPERTY(BlueprintAssignable, Category = "Focus")
	FOnNewActionsSets OnNewActionsSets;

	FOnFocusActionPerformed OnActionPerformed;

public:

	UFocusComponent();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UFocusComponent* GetFocusComponent(const AActor* Owner);

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual bool PerformActionType(FFocusPerformAction FocusPerformAction) const;

public:

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	void SetFocusOnActor(AActor* ActorToFocus, float Distance);


	UFUNCTION(BlueprintCallable, Category = "ActionType")
	int32 GetActionsSetsLenght() const{ return ActionsSets.Num();}

	UFUNCTION(BlueprintCallable)
	void SetOwnerController(AController* Controller);

	UFUNCTION(BlueprintCallable)
	virtual void SetOwnerPawn(APawn* NewOwnerPawn, const bool bIsIgnored = true);

	UFUNCTION()
	APawn* GetOwnerPawn() const;

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "Unset this Component to Focus Mode"), Category = "Actions")
	UFocusableComponent* GetFocusedComponent() const { return FocusInfo.FocusedComponent; }

	UFUNCTION(BlueprintCallable, meta = (Tooltip = "Unset this Component to Focus Mode"), Category = "Actions")
	AActor* GetFocusedActor() const { return FocusInfo.FocusedActor; }

	//UActorComponent Interface

	virtual void Deactivate() override;
	//End UActorComponent Interface


	UFUNCTION(BlueprintCallable, Category = "ActionType")
	virtual FName GetCurrentActionFriendlyName(int32 const InteractionsSetIndex) const;

	UFUNCTION(BlueprintCallable, Category = "ActionType")
	virtual void GetFriendlyNames(TArray<FFocusActionFriendlyName>& OutFriendlyNames) const;
	
	UFUNCTION(BlueprintCallable, Category = "ActionType")
	virtual bool MakeInteractionParams(int32 const InteractionsSetIndex,FFocusActionParams& Params);

	virtual AController* GetController() const;

	virtual AController* GetActionController() const;

	virtual APawn* GetPawn() const;

	virtual APawn* GetActionPawn() const;
	
	UFUNCTION(BlueprintCallable, Category = "ActionType")
	UActionsSet* GetActionsSet(int32 actionSetIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual bool GetPerformActionWithIndex(int32 actionSetIndex, FFocusPerformAction& FocusPerformAction) const;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual bool GetPerformAction(UActionsSet* ActionSet, FFocusPerformAction& FocusPerformAction) const;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Actions")
	virtual void Server_SetNewActionsSetDeleteOlds(TArray<UActionsSet*>const & NewActionSet);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual void SetNewActionsSetDeleteOlds(TArray<UActionsSet*> NewActionSet);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual void SetActionsSet(UActionsSet* NewActionSet, const int32 Index = 0);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Actions")
	virtual void Server_SetActionsSet(UActionsSet* NewActionSet,const int32 Index = 0);
	
	virtual bool IsAllowedByFilters(AActor* Actor, UFocusableComponent* FocusableComponent) const;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual bool CanExecuteAnyAction(const TArray<UActionsSet*>& InActionsSets, UActionableType* ActionableType, AActor* ActionActor, AActor* ActionableActor) const;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual bool PerformAction(int32 ActionsSetIndex);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual bool PerformThisAction(UActionType* Action);
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual bool PerformActionAgainstActor(UActionType* Action, AActor* ActorToFocus, float Distance);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual bool PerformActionWithFocusPerformAction(FFocusPerformAction FocusPerformAction);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual bool PerformActionWithClientFocus(int32 ActionsSetIndex);

	virtual bool IsLocallyControlled() const;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPerformAction(FFocusPerformAction FocusPerformAction);

	UFUNCTION()
	void OnRep_ActionsSets();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
