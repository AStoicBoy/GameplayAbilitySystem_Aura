// Copyright Dorian Souc

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageSignature, FUIWidgetRow, Message);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	void ProcessTickFunction(const FOnAttributeChangeData& Data, FTimerHandle& TimerHandle, FOnAttributeChangedSignature& OnValueGhostChanged, const FOnAttributeChangedSignature& OnValueChanged, bool& bFirstValueUpdate, float& CurrentValue, float& OldValue, float& NewValue);
	void GhostTickFunction(const FOnAttributeChangedSignature& OnValueGhostChanged, FTimerHandle& TimerHandle, float& CurrentValue, float& OldValue, float& NewValue);


	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Message")
	FOnMessageSignature MessageWidgetRowDelegate;

	//Ghost
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthGhostChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaGhostChanged;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* Datatable, const FGameplayTag& Tag);

	bool bFirstHealthUpdate = true;
	bool bFirstManaUpdate = true;
	float OldHealth;
	float NewHealth;
	float CurrentHealth;
	float ElapsedTime;
	float CurrentMana;
	float NewMana;
	float OldMana;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	float FixedGlobeDelay = 0.4f;
	FTimerHandle HealthTimerHandle;
	FTimerHandle ManaTimerHandle;


};

	template <typename T>
	T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
	{
		return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	}
