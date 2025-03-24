// Copyright Dorian Souc


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "InterchangeResult.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	if (!EffectAssetTags.IsBound())
	{
		EffectAssetTags.Clear();
		OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		// GameplayTags.Attribute_Secondary_Armor.ToString();
		// GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Orange, FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attribute_Secondary_Armor.ToString()));
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

