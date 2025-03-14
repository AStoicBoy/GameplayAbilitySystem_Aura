// Copyright Dorian Souc


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const TObjectPtr<UAuraAttributeSet> AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	MessageWidgetRowDelegate.Clear();
	const TObjectPtr<UAuraAttributeSet> AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			ProcessTickFunction(Data,HealthTimerHandle,OnHealthGhostChanged, OnHealthChanged, bFirstHealthUpdate, CurrentHealth, OldHealth, NewHealth);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			ProcessTickFunction(Data, ManaTimerHandle,OnManaGhostChanged,OnManaChanged, bFirstManaUpdate, CurrentMana, OldMana, NewMana);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	if (!Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.IsBound())
	{
		Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
	[this](const FGameplayTagContainer& AssetTags)
	{
		
		for (const FGameplayTag& Tag : AssetTags)
		{
			// Determine if this tag matches TagToCheck, expanding out parent tags
			//"A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False
			// For example: Message.HealthPotion "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
				const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
				MessageWidgetRowDelegate.Broadcast(*Row);
				}
		}
	}
	);
	}
}

void UOverlayWidgetController::GhostTickFunction(const FOnAttributeChangedSignature& OnValueGhostChanged, FTimerHandle& TimerHandle, float& CurrentValue, float& OldValue, float& NewValue)
{
	ElapsedTime += 0.1f; // Increment elapsed time

	// Interpolate value over 2 seconds
	float Alpha = FMath::Clamp(ElapsedTime / 2.0f, 0.0f, 1.0f);
	CurrentValue = FMath::Lerp(OldValue, NewValue, Alpha);

	// Broadcast interpolated value
	OnValueGhostChanged.Broadcast(CurrentValue);

	// Stop the timer after 2 seconds
	if (ElapsedTime >= 2.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void UOverlayWidgetController::ProcessTickFunction(const FOnAttributeChangeData& Data, FTimerHandle& TimerHandle, FOnAttributeChangedSignature& OnValueGhostChanged, const FOnAttributeChangedSignature& OnValueChanged, bool& bFirstValueUpdate, float& CurrentValue, float& OldValue, float& NewValue)
{
	if (UWorld* World = GetOuter()->GetWorld())
	{
		OldValue = Data.OldValue;
		NewValue = Data.NewValue;
	
		if (World->GetTimerManager().IsTimerActive(TimerHandle))
		{
			CurrentValue = OldValue;
			ElapsedTime = 0.0f;
		}
		else if (bFirstValueUpdate == true)
		{
			// Clear previous timer if it's running
			World->GetTimerManager().ClearTimer(TimerHandle);

			// Start the timer correctly
			World->GetTimerManager().SetTimer(
			TimerHandle,
			[this, OnValueGhostChanged, &TimerHandle, &CurrentValue, &OldValue, &NewValue]()  
			{  
				GhostTickFunction(OnValueGhostChanged, TimerHandle, CurrentValue, OldValue, NewValue);  
			},
			0.1f,  // Interval
			true,   // Looping
			0.0f    // Start immediately
			);
		}
		else
		{
			// Clear previous timer if it's running
			World->GetTimerManager().ClearTimer(TimerHandle);

			// Start the timer correctly
			World->GetTimerManager().SetTimer(
			TimerHandle,
			[this, OnValueGhostChanged, &TimerHandle, &CurrentValue, &OldValue, &NewValue]()  
			{  
				GhostTickFunction(OnValueGhostChanged, TimerHandle, CurrentValue, OldValue, NewValue);
			},
			0.1f,  // Interval
			true,   // Looping
			FixedGlobeDelay
			);
		}
					
		// Broadcast the new value change immediately
		OnValueChanged.Broadcast(Data.NewValue);
		bFirstValueUpdate = false;
	}
}