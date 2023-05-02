// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DefaultDataAssets/AbilityBindingDataAsset.h"

TSubclassOf<UProtelumGameplayAbility> UAbilityBindingDataAsset::GetEmptyAbilityClass() const
{
	return EmptyGameplayAbilityClass;
}

TArray<FGameplayAbilityInfo> UAbilityBindingDataAsset::GetAbilities()
{
	return BoundAbilities;
}