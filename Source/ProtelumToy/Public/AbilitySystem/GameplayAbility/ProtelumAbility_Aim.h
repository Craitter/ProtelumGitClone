// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/ProtelumGameplayAbility.h"
#include "ProtelumAbility_Aim.generated.h"

class AProtelumCharacter;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UProtelumAbility_Aim : public UProtelumGameplayAbility
{
	GENERATED_BODY()

public:
	UProtelumAbility_Aim();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	TObjectPtr<AProtelumCharacter> Character = {nullptr};

	//Time in Seconds until the Long Aiming Ends
	UPROPERTY(EditAnywhere)
	float TimeUntilAimingEnds = 0.0f;
	
	FGameplayTag LongAimTag;
	FGameplayTag ShortAimTag;
	
	//Callback functions to End the Ability It was held alive because there was a LongAimTag on the ASC
	FTimerHandle DelayAimTimer;
	void OnAimEnd();

private:
	FGameplayAbilitySpecHandle OutHandle;
	FGameplayAbilityActorInfo* OutActorInfo;
	FGameplayAbilityActivationInfo OutActivationInfo;
	bool bOutReplicateEndAbility;
	bool bOutWasCancelled;

	bool bDelayAimEnd = false;
};
