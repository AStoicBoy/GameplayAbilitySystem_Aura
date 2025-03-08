// Copyright Dorian Souc


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const TObjectPtr<UAuraAttributeSet> AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());

	UE_LOG(LogTemp, Warning, TEXT("Broadcasting Initial Health: %f"), AuraAttributeSet->GetHealth());
	UE_LOG(LogTemp, Warning, TEXT("Broadcasting Initial Mana: %f"), AuraAttributeSet->GetMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const TObjectPtr<UAuraAttributeSet> AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	MessageWidgetRowDelegate.Clear();
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
	

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
				//UE_LOG(LogTemp, Warning, TEXT("Delegate has %d bindings"), MessageWidgetRowDelegate.GetAllObjects().Num());
				//UE_LOG(LogTemp, Warning, TEXT("Effect Applied in ASC: %s"), *GetNameSafe(this));
			
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

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
