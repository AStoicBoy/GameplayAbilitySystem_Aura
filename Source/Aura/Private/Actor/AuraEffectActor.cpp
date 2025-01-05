// Copyright Dorian Souc

#include "Actor/AuraEffectActor.h"

#include "Components/SphereComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ActiveGameplayEffectHandle.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Scene Component"));

}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	TObjectPtr<UAbilitySystemComponent> TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target); // Get ASC though ASInterface smoothly

	if (TargetASC == nullptr) return;
	check(GameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContextHandle);
	const FGameplayEffectSpec EffectSpec = *EffectSpecHandle.Data.Get();
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(EffectSpec);

	const bool bIsInfinite = EffectSpec.Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC); // Adding to Tmap ActiveEffecHanldes: Active effect handle + TargetASC
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (ArrayGameplayEffectClass.Num() == 0) return;
	
	for (TSubclassOf<UGameplayEffect> EffectClass : ArrayGameplayEffectClass)
	{
		if (!IsValid(EffectClass)) return;
		
		if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectClass);	
		}
		if (IsValid(HasDurationGameplayEffectClass))
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}
		if (IsValid(InfiniteGameplayEffectClass))
		{
			ApplyEffectToTarget(TargetActor, EffectClass);
		}

	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{

	for (TSubclassOf<UGameplayEffect> EffectClass : ArrayGameplayEffectClass)
	{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, EffectClass);
	}	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, EffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, EffectClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		TObjectPtr<UAbilitySystemComponent> TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;

		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
	}
}

