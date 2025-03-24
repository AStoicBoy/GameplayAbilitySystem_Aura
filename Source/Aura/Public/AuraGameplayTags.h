// Copyright Dorian Souc

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 *
 * Singleton containing naive Gameplay Tags
 */

struct FAuraGameplayTags
{
	public:
	FAuraGameplayTags();
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
	FGameplayTag Attribute_Secondary_Armor;

	
	protected:

	private:
	static FAuraGameplayTags GameplayTags;
};
