// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Calculations/ModMagCalc/MMC_Cooldown.h"

#include "AbilitySystem/GameplayAbility/ProtelumGameplayAbility.h"

float UMMC_Cooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UProtelumGameplayAbility* Ability = Cast<UProtelumGameplayAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated());

	if (!Ability)
	{
		return 0.0f;
	}

	return Ability->GetCooldownDuration().GetValueAtLevel(Ability->GetAbilityLevel());
}
