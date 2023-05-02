// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageExecuteCalculation.generated.h"

/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UDamageExecuteCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UDamageExecuteCalculation();
	static bool IsACriticalHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                           const FAggregatorEvaluateParameters& EvaluationParameters);

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
