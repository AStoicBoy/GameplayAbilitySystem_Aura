// Copyright Dorian Souc

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"

#include "AuraCharacter.generated.h"



class UCameraComponent; class USpringArmComponent;
	
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Combat Interface
	virtual int32 GetPlayerLevel() override;

private:

	virtual void InitAbilityActorInfo() override;
	bool bAbilityActorInfoInitialized = false;

	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

};
