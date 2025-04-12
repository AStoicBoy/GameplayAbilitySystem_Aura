// Copyright Dorian Souc

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"

#include "AuraEnemy.generated.h"
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	virtual void BeginPlay() override;
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	UPROPERTY(EditAnywhere, Category = "Cursor Outine");
	int32 CustomDepthRed = 255;

	//Combat Interface
	virtual int32 GetPlayerLevel() override; // This is a getter

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
protected:

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults");
	int32 Level = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults");
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	TObjectPtr<UWidgetComponent> HealthBar;


	
private:
	
	
	

	
};
