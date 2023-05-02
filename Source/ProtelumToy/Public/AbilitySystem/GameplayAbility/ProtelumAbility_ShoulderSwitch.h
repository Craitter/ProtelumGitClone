// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/ProtelumGameplayAbility.h"
#include "ProtelumAbility_ShoulderSwitch.generated.h"

/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UProtelumAbility_ShoulderSwitch : public UProtelumGameplayAbility
{
	GENERATED_BODY()

public:
	UProtelumAbility_ShoulderSwitch();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectToApply = {nullptr}; 
};
