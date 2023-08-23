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

public:

	UProtelumAbilitySystemComponent();
	
	void SetIsPlayerControlled();

	virtual void AbilityLocalInputPressed(int32 InputID) override;

protected:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;

	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;

private:
	FGameplayTag PlayerControlledTag;
};
