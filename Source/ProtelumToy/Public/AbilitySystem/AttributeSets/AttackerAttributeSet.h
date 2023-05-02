// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AttackerAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UAttackerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Clamping and other changes that might be mandatory for the Attribute just before it gets applied ...
	//anything that recalculates the CurrentValue from all of the modifiers like
	//GameplayEffectExecutionCalculations and ModifierMagnitudeCalculations need to implement clamping again.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	//Base Value changes of Instant Gameplay Effects should be adjusted right here
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	//Used for example: Only strongest slow effect applies
	// virtual void OnAttributeAggregatorCreated(const FGameplayAttribute& Attribute, FAggregator* NewAggregator) const override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attacker", ReplicatedUsing = OnRep_CriticalHitChance)
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAttackerAttributeSet, CriticalHitChance)

	UPROPERTY(BlueprintReadOnly, Category = "Attacker", ReplicatedUsing = OnRep_ShieldPenetration)
	FGameplayAttributeData ShieldPenetration;
	ATTRIBUTE_ACCESSORS(UAttackerAttributeSet, ShieldPenetration)

	UPROPERTY(BlueprintReadOnly, Category = "Attacker", ReplicatedUsing = OnRep_ResistancePenetration)
	FGameplayAttributeData ResistancePenetration;
	ATTRIBUTE_ACCESSORS(UAttackerAttributeSet, ResistancePenetration)

	
protected:
	//Adjusts an Attribute to fit the Percentage it had before the MaxAttribute change
	// void AdjustAttributeForMaxChange(const FGameplayAttributeData & AffectedAttribute, const FGameplayAttributeData & MaxAttribute, float NewMaxValue, const FGameplayAttribute & AffectedAttributeProperty) const;
	
	UFUNCTION()
	virtual void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	UFUNCTION()
	virtual void OnRep_ShieldPenetration(const FGameplayAttributeData& OldShieldPenetration) const;
	UFUNCTION()
	virtual void OnRep_ResistancePenetration(const FGameplayAttributeData& OldResistancePenetration) const;
};
