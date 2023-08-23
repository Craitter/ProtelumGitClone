// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityBindingDataAsset.generated.h"


class UProtelumAbilitySystemComponent;
class UProtelumGameplayAbility;
class UInputAction;

USTRUCT(BlueprintType)
struct FGameplayAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = BindInfo)
	TSubclassOf<UProtelumGameplayAbility> GameplayAbilityClass;

	UPROPERTY(EditAnywhere, Category = BindInfo)
	TSoftObjectPtr<UInputAction> InputAction;

	UPROPERTY()//Runtime Todo: Change to Handle, this is much more efficient
	FGameplayAbilitySpecHandle SpecHandle;
	
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "Ability.AbilitySlot"))
	FGameplayTag SlotTag;
};
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UAbilityBindingDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	TSubclassOf<UProtelumGameplayAbility> GetEmptyAbilityClass() const;
	
	TArray<FGameplayAbilityInfo> GetAbilities();

private:
	/** Abilities to Bind to a Character */
	UPROPERTY(EditAnywhere, Category = AbilitySet)
	TArray<FGameplayAbilityInfo> BoundAbilities;

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	TSubclassOf<UProtelumGameplayAbility> EmptyGameplayAbilityClass;
};
