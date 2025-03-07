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
	static TMap<FGameplayTag, float> LastEffectTime; // Tracks the last time an effect was processed
	float CurrentTime = GetWorld()->GetTimeSeconds(); // Get the game time in seconds

	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	for (const FGameplayTag& Tag : TagContainer)
	{
		// Prevents the same effect from being applied twice in a short time
		if (LastEffectTime.Contains(Tag) && (CurrentTime - LastEffectTime[Tag]) < 0.1f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Skipping duplicate effect application: %s"), *Tag.ToString());
			return;
		}

		LastEffectTime.Add(Tag, CurrentTime); // Update effect timestamp

		UE_LOG(LogTemp, Warning, TEXT("Effect Applied in ASC: %s"), *GetNameSafe(this));
		EffectAssetTags.Broadcast(TagContainer);
	}
}

