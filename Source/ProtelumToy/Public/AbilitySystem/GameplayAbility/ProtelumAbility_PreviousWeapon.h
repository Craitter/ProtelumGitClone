// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/ProtelumGameplayAbility.h"
#include "ProtelumAbility_PreviousWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UProtelumAbility_PreviousWeapon : public UProtelumGameplayAbility
{
	GENERATED_BODY()

	UProtelumAbility_PreviousWeapon();
	//CanActivate? Todo: Future more dynamic depending on animations 
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
