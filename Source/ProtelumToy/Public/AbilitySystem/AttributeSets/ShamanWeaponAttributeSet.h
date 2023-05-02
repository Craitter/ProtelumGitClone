// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ShamanWeaponAttributeSet.generated.h"

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
class PROTELUMTOY_API UShamanWeaponAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
protected:

	UFUNCTION()
	virtual void OnRep_DamageProjectileMaxAmmo(const FGameplayAttributeData& OldDamageProjectileMaxAmmo);

	UFUNCTION()
	virtual void OnRep_DamageProjectileAmmo(const FGameplayAttributeData& OldDamageProjectileMaxAmmo);

	UFUNCTION()
	virtual void OnRep_DamageProjectileRegeneration(const FGameplayAttributeData& OldDamageProjectileMaxAmmo);

	UFUNCTION()
	virtual void OnRep_ProjectileDamage(const FGameplayAttributeData& OldDamageProjectileMaxAmmo);

	UFUNCTION()
	virtual void OnRep_HealProjectileMaxAmmo(const FGameplayAttributeData& OldDamageProjectileMaxAmmo);

	UFUNCTION()
	virtual void OnRep_HealProjectileAmmo(const FGameplayAttributeData& OldDamageProjectileMaxAmmo);

	UFUNCTION()
	virtual void OnRep_HealProjectileRegeneration(const FGameplayAttributeData& OldDamageProjectileMaxAmmo);

	UFUNCTION()
	virtual void OnRep_ProjectileHeal(const FGameplayAttributeData& OldDamageProjectileMaxAmmo);

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

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_DamageProjectileMaxAmmo)
	FGameplayAttributeData DamageProjectileMaxAmmo;
	ATTRIBUTE_ACCESSORS(UShamanWeaponAttributeSet, DamageProjectileMaxAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_DamageProjectileAmmo)
	FGameplayAttributeData DamageProjectileAmmo;
	ATTRIBUTE_ACCESSORS(UShamanWeaponAttributeSet, DamageProjectileAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_DamageProjectileRegeneration)
	FGameplayAttributeData DamageProjectileRegeneration; //Per Second
	ATTRIBUTE_ACCESSORS(UShamanWeaponAttributeSet, DamageProjectileRegeneration)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_ProjectileDamage)
	FGameplayAttributeData ProjectileDamage;
	ATTRIBUTE_ACCESSORS(UShamanWeaponAttributeSet, ProjectileDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_HealProjectileMaxAmmo)
	FGameplayAttributeData HealProjectileMaxAmmo;
	ATTRIBUTE_ACCESSORS(UShamanWeaponAttributeSet, HealProjectileMaxAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_HealProjectileAmmo)
	FGameplayAttributeData HealProjectileAmmo;
	ATTRIBUTE_ACCESSORS(UShamanWeaponAttributeSet, HealProjectileAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_HealProjectileRegeneration)
	FGameplayAttributeData HealProjectileRegeneration; //Per Second
	ATTRIBUTE_ACCESSORS(UShamanWeaponAttributeSet, HealProjectileRegeneration)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_ProjectileHeal)
	FGameplayAttributeData ProjectileHeal;
	ATTRIBUTE_ACCESSORS(UShamanWeaponAttributeSet, ProjectileHeal)


	
};
