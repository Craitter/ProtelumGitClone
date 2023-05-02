// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DefaultDataAssets/AbilityLibraryDataAsset.h"

#include "AbilitySystem/GameplayAbility/ProtelumGameplayAbility.h"

TArray<FOwnedAbility> UAbilityLibraryDataAsset::GetOwnableAbilities()
{
	return OwnableAbilities;
}

FGameplayTagContainer UAbilityLibraryDataAsset::GetOwnableAbilitiesTags()
{
	FGameplayTagContainer OwnableTags;
	for(FOwnedAbility OwnedAbility : OwnableAbilities)
	{
		OwnableTags.AddTag(OwnedAbility.GameplayTag);
	}
	return OwnableTags;
}

TArray<TSubclassOf<UProtelumGameplayAbility>> UAbilityLibraryDataAsset::GetOwnableAbilityClasses()
{
	TArray<TSubclassOf<UProtelumGameplayAbility>> OwnableAbilityClasses;
	for(FOwnedAbility OwnedAbility : OwnableAbilities)
	{
		OwnableAbilityClasses.Add(OwnedAbility.GameplayAbilityClass);
	}
	return OwnableAbilityClasses;
}

FGameplayTag UAbilityLibraryDataAsset::GetOwnableTagByAbilityClass(TSubclassOf<UProtelumGameplayAbility> AbilityClass)
{
	FGameplayTag OwnedTag = FGameplayTag::EmptyTag;
	for(FOwnedAbility OwnedAbility : OwnableAbilities)
	{
		if(OwnedAbility.GameplayAbilityClass == AbilityClass)
		{
			OwnedTag = OwnedAbility.GameplayTag;
		}
	}
	return OwnedTag;
}

TSubclassOf<UProtelumGameplayAbility> UAbilityLibraryDataAsset::GetOwnableAbilityClassByTag(const FGameplayTag* Tag)
{
	TSubclassOf<UProtelumGameplayAbility> AbilityClass = {nullptr};
	for(FOwnedAbility OwnedAbility: OwnableAbilities)
	{
		if(OwnedAbility.GameplayTag == *Tag)
		{
			AbilityClass = OwnedAbility.GameplayAbilityClass;
		}
	}
	return AbilityClass;
}
