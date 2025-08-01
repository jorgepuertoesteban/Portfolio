// Created by Jorge Puerto. All Rights Reserved.


#include "Subsystems/CharacterCreationSubsystem.h"
#include "Engine/GameInstance.h"
#include "Logs/CharacterCreationLog.h"

static const FString ContextString(TEXT("Character Creation"));

void UCharacterCreationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	DaoOwner = Cast<ICharacterCreationDAOOwner>(GetGameInstance());

	if (!DaoOwner)
	{
		UE_LOG(CharacterCreationLog, Error, TEXT("UCharacterCreationSubsystem::Initialize() Game Instance Doesn't implement ICharacterCreationDAOOwner!!"));
	}
}