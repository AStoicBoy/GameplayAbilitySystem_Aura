// Copyright Dorian Souc

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"

#include "AuraEnemy.generated.h"

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
	
protected:

	virtual void InitAbilityActorInfo() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults");
	int32 Level = 1;
	
private:
	
	
	

	
};
