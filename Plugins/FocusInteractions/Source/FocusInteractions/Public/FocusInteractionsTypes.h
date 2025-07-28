// Created by Bionic Ape. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FocusInteractionsTypes.generated.h"

class AActor;
class UFocusableComponent;
class UActionType;
class AController;
class UFocusComponent;
class UActionableType;
class UActionsSet;
class APawn;

USTRUCT(BlueprintType)
struct FActionByActionsSetResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Focus")
	UActionsSet* ActionsSet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Focus")
	UActionType* Action;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Focus")
	int32 ActionSetIndex;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Actions")
	AActor* ActionableActor;

	FActionByActionsSetResult() : ActionsSet(nullptr), Action(nullptr), ActionSetIndex(0), ActionableActor(nullptr)
	{

	}

	FActionByActionsSetResult(UActionsSet* NewActionsSet, UActionType* NewAction, int32 NewActionSetIndex, AActor* NewActionableActor) : ActionsSet(NewActionsSet), Action(NewAction), ActionSetIndex(NewActionSetIndex), ActionableActor(NewActionableActor)
	{

	}
};


USTRUCT(BlueprintType)
struct FFocusInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Focus")
	UFocusComponent* FocusComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Focus")
	UFocusableComponent* FocusedComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Focus")
	AActor* FocusedActor = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Focus")
	float Distance = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Focus")
	TArray<FActionByActionsSetResult> Actions;

	bool operator==(const FFocusInfo& Other) const
	{
		return	FocusedComponent == Other.FocusedComponent;
	}

	bool operator!=(const FFocusInfo& Other) const
	{
		return !operator==(Other);
	}

	FFocusInfo() {}

	void Reset()
	{
		FocusedActor = nullptr;
		FocusedComponent = nullptr;
		Actions.Empty();
	}

};

USTRUCT(BlueprintType)
struct FFocusActionParams
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* CallerActor;

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UFocusComponent* CallerFocus;

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* ReceiverActor;

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UFocusableComponent* ReceiverFocused;

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UActionType* Action;

	FFocusActionParams() : CallerActor(nullptr), CallerFocus(nullptr), ReceiverActor(nullptr), ReceiverFocused(nullptr), Action(nullptr)
	{

	}
};

USTRUCT(BlueprintType)
struct FFocusActionFriendlyName
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName Name;

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsValid;

	FFocusActionFriendlyName() : Name(NAME_None), bIsValid(false)
	{
	}
	FFocusActionFriendlyName(FName InName, bool bInIsValid) : Name(InName), bIsValid(bInIsValid)
	{
	}
};

USTRUCT(BlueprintType)
struct FOCUSINTERACTIONS_API FFocusPerformAction
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UActionsSet* ActionsSet;

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UActionType* ActionType;

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UActionableType* ActionableType;

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UFocusableComponent* Focusable;

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	APawn* ActionPawn;

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AController* ActionController;

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Distance;

	UPROPERTY(Category = "FocusInteractions", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* FocusedActor;

	FFocusPerformAction() : ActionsSet(nullptr), ActionType(nullptr), ActionableType(nullptr), Focusable(nullptr), ActionPawn(nullptr), ActionController(nullptr), Distance(0.0f), FocusedActor(nullptr)
	{
	}

	FString ToString() const;

	void Reset();
};