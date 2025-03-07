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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

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


	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnManaChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Message")
	FOnMessageSignature MessageWidgetRowDelegate;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void ManaChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* Datatable, const FGameplayTag& Tag);
	

};

	template <typename T>
	T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
	{
		if (!DataTable)
		{
			UE_LOG(LogTemp, Error, TEXT("DataTable is nullptr in GetDataTableRowByTag"));
			return nullptr;
		}

		TArray<FName> RowNames = DataTable->GetRowNames();
		int32 FoundCount = 0;
		T* FoundRow = nullptr;

		for (const FName& RowName : RowNames)
		{
			T* Row = DataTable->FindRow<T>(RowName, TEXT("Checking duplicates"));
			if (Row && Row->MessageTag.MatchesTag(Tag))  // <-- Correctly checking against MessageTag
			{
				FoundCount++;
				FoundRow = Row;
			}
		}

		if (FoundCount > 1)
		{
			UE_LOG(LogTemp, Error, TEXT("Duplicate GameplayTag '%s' found %d times in DataTable!"), *Tag.ToString(), FoundCount);
		}

		return FoundRow;
	}
