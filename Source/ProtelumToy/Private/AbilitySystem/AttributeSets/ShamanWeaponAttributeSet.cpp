// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/ShamanWeaponAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

void UShamanWeaponAttributeSet::OnRep_DamageProjectileMaxAmmo(const FGameplayAttributeData& OldDamageProjectileMaxAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShamanWeaponAttributeSet, DamageProjectileMaxAmmo, OldDamageProjectileMaxAmmo);
}

void UShamanWeaponAttributeSet::OnRep_DamageProjectileAmmo(const FGameplayAttributeData& OldDamageProjectileAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShamanWeaponAttributeSet, DamageProjectileAmmo, OldDamageProjectileAmmo);
}

void UShamanWeaponAttributeSet::OnRep_DamageProjectileRegeneration(
	const FGameplayAttributeData& OldDamageProjectileRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShamanWeaponAttributeSet, DamageProjectileRegeneration, OldDamageProjectileRegeneration);
}

void UShamanWeaponAttributeSet::OnRep_ProjectileDamage(const FGameplayAttributeData& OldProjectileDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShamanWeaponAttributeSet, ProjectileDamage, OldProjectileDamage);
}

void UShamanWeaponAttributeSet::OnRep_HealProjectileMaxAmmo(const FGameplayAttributeData& OldHealProjectileMaxAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShamanWeaponAttributeSet, HealProjectileMaxAmmo, OldHealProjectileMaxAmmo);
}

void UShamanWeaponAttributeSet::OnRep_HealProjectileAmmo(const FGameplayAttributeData& OldHealProjectileAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShamanWeaponAttributeSet, HealProjectileAmmo, OldHealProjectileAmmo);
}

void UShamanWeaponAttributeSet::OnRep_HealProjectileRegeneration(
	const FGameplayAttributeData& OldHealProjectileRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShamanWeaponAttributeSet, HealProjectileRegeneration, OldHealProjectileRegeneration);
}

void UShamanWeaponAttributeSet::OnRep_ProjectileHeal(const FGameplayAttributeData& OldProjectileHeal)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UShamanWeaponAttributeSet, ProjectileHeal, OldProjectileHeal);
}

void UShamanWeaponAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UShamanWeaponAttributeSet, DamageProjectileMaxAmmo, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UShamanWeaponAttributeSet, DamageProjectileAmmo, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UShamanWeaponAttributeSet, DamageProjectileRegeneration, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UShamanWeaponAttributeSet, ProjectileDamage, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UShamanWeaponAttributeSet, HealProjectileMaxAmmo, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UShamanWeaponAttributeSet, HealProjectileAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UShamanWeaponAttributeSet, HealProjectileRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UShamanWeaponAttributeSet, ProjectileHeal, COND_None, REPNOTIFY_Always);
}

void UShamanWeaponAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	const TWeakObjectPtr<UAbilitySystemComponent> AbilitySystem = GetOwningAbilitySystemComponent();
	FGameplayAttribute AffectedAttributeProperty;
	float MaxChangeAffectionDelta = 0.0f;
	if(Attribute == GetDamageProjectileMaxAmmoAttribute())
	{
		if(NewValue < GetDamageProjectileAmmo())
		{
			MaxChangeAffectionDelta = NewValue - GetDamageProjectileAmmo();
			AffectedAttributeProperty = GetDamageProjectileAmmoAttribute();
		}
	}
	else if(Attribute == GetHealProjectileMaxAmmoAttribute())
	{
		if(NewValue < GetHealProjectileAmmo())
		{
			MaxChangeAffectionDelta = NewValue - GetHealProjectileAmmo();
			AffectedAttributeProperty = GetHealProjectileAmmoAttribute();
		}
	}
	else if(Attribute == GetDamageProjectileAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetDamageProjectileMaxAmmo());
	}
	else if(Attribute == GetDamageProjectileRegenerationAttribute())
	{
		// Todo: Limit to something
	}
	else if(Attribute == GetHealProjectileAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetHealProjectileMaxAmmo());
	}
	else if(Attribute == GetHealProjectileRegenerationAttribute())
	{
		// Todo: Limit to something
	}

	if(AffectedAttributeProperty != nullptr)
	{
		AbilitySystem->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, MaxChangeAffectionDelta);
	}
}

void UShamanWeaponAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetDamageProjectileAmmoAttribute())
	{
		SetDamageProjectileAmmo(FMath::Clamp(GetDamageProjectileAmmo(), 0.0f, GetDamageProjectileMaxAmmo()));
	}
	else if(Data.EvaluatedData.Attribute == GetDamageProjectileRegenerationAttribute())
	{
		// Todo: Limit to something
	}
	else if(Data.EvaluatedData.Attribute == GetHealProjectileAmmoAttribute())
	{
		SetHealProjectileAmmo(FMath::Clamp(GetHealProjectileAmmo(), 0.0f, GetHealProjectileMaxAmmo()));
	}
	else if(Data.EvaluatedData.Attribute == GetHealProjectileRegenerationAttribute())
	{
		// Todo: Limit to something
	}
}
