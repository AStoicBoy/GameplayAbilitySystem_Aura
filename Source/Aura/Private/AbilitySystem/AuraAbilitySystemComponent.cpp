// Copyright Dorian Souc


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "InterchangeResult.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	if (!EffectAssetTags.IsBound())
	{
		EffectAssetTags.Clear();
		OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
		UE_LOG(LogTemp, Warning, TEXT("AbilityActorInfoSet() called in %s"), *GetNameSafe(this));
	}
}

void UAuraAbilitySystemComponent::EffectApplied(
	UAbilitySystemComponent* AbilitySystemComponent, 
	const FGameplayEffectSpec& EffectSpec, 
	FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}

