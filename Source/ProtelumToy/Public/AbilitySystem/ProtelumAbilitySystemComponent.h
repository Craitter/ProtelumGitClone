// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProtelumAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UProtelumAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	UProtelumAbilitySystemComponent();
	
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;

	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;

public:
	void SetIsPlayerControlled();
private:
	FGameplayTag PlayerControlledTag;
};
