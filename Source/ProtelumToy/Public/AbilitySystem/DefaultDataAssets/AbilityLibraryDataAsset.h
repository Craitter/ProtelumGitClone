// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityLibraryDataAsset.generated.h"

class UProtelumGameplayAbility;

USTRUCT(BlueprintType)
struct FOwnedAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UProtelumGameplayAbility> GameplayAbilityClass;

	UPROPERTY(EditAnywhere, Meta = (Categories = "AbilityClasses"))
	FGameplayTag GameplayTag;
	
};
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UAbilityLibraryDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	TArray<FOwnedAbility> GetOwnableAbilities();
	FGameplayTagContainer GetOwnableAbilitiesTags();
	TArray<TSubclassOf<UProtelumGameplayAbility>> GetOwnableAbilityClasses();
	FGameplayTag GetOwnableTagByAbilityClass(TSubclassOf<UProtelumGameplayAbility> AbilityClass);
	TSubclassOf<UProtelumGameplayAbility> GetOwnableAbilityClassByTag(const FGameplayTag* Tag);


private:
	UPROPERTY(EditAnywhere)
	TArray<FOwnedAbility> OwnableAbilities;
};
