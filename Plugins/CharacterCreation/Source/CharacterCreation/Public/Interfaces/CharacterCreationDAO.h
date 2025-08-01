// Created by Jorge Puerto. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "Exchanges/CharacterCreationExchange.h"
#include "CharacterCreationDAO.generated.h"


class UCharacterCreator;

/**
*
*/
UINTERFACE(Blueprintable)
class CHARACTERCREATION_API UCharacterCreationDAO : public UInterface
{
	GENERATED_BODY()

};

class ICharacterCreationDAO
{
	GENERATED_BODY()

public:

	virtual void SaveCharacterCreator(const UCharacterCreator* CharacterCreator, const FSaveCharacterCreatorDelegate& Delegate) = 0;

	virtual void LoadCharacterCreator(const int32 Id, const FLoadCharacterCreatorDelegate& Delegate) = 0;
};

#pragma region DAOOwner


UINTERFACE(Blueprintable)
class CHARACTERCREATION_API UCharacterCreationDAOOwner : public UInterface
{
	GENERATED_BODY()

};


class ICharacterCreationDAOOwner
{
	GENERATED_BODY()
public:
	virtual ICharacterCreationDAO* GetCharacterCreationDAO() const = 0;
};
#pragma endregion