// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "HealthAttributeSet.generated.h"

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
class PROTELUMTOY_API UHealthAttributeSet : public UAttributeSet
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
	
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_HealthRegenerationRate)
	FGameplayAttributeData HealthRegenerationRate;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, HealthRegenerationRate)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Shield)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Shield)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_ShieldRegenerationRate)
	FGameplayAttributeData ShieldRegenerationRate;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, ShieldRegenerationRate)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Damage)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_DamageResistance)
	FGameplayAttributeData DamageResistance;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, DamageResistance)
	
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_DamageVulnerability)
	FGameplayAttributeData DamageVulnerability;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, DamageVulnerability)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_CriticalHitResistance)
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, CriticalHitResistance)

protected:
	//Adjusts an Attribute to fit the Percentage it had before the MaxAttribute change
	void AdjustAttributeForMaxChange(const FGameplayAttributeData & AffectedAttribute, const FGameplayAttributeData & MaxAttribute, float NewMaxValue, const FGameplayAttribute & AffectedAttributeProperty) const;
	
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHeath);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_HealthRegenerationRate(const FGameplayAttributeData& OldHealthRegenerationRate);

	UFUNCTION()
	virtual void OnRep_Shield(const FGameplayAttributeData& OldShield);

	UFUNCTION()
	virtual void OnRep_DamageResistance(const FGameplayAttributeData& OldDamageResistance);

	UFUNCTION()
	virtual void OnRep_ShieldRegenerationRate(const FGameplayAttributeData& OldShieldRegenerationRate);
	
	UFUNCTION()
	virtual void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance);

	UFUNCTION()
	virtual void OnRep_DamageVulnerability(const FGameplayAttributeData& OldDamageVulnerability);
};
