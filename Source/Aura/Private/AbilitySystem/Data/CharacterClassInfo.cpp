// Copyright Dorian Souc


#include "AbilitySystem/Data/CharacterClassInfo.h"

#include "GameplayEffect.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetCharacterClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}