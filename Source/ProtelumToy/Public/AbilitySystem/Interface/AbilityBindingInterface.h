// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilityBindingInterface.generated.h"

struct FGameplayAbilitySpec;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAbilityBindingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROTELUMTOY_API IAbilityBindingInterface
{
	GENERATED_BODY()

	// Interface Functions to check if the Ability which was given to or removed from the AbilitySystemComponent
	// needs to be (un)bound to Input
public:
	virtual void BindAbility(FGameplayAbilitySpec& AbilitySpec) = 0;

	virtual void UnbindAbility(FGameplayAbilitySpec& AbilitySpec) = 0;
};
