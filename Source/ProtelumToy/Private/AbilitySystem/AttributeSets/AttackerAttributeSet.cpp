// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/AttackerAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

void UAttackerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAttackerAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UAttackerAttributeSet, ShieldPenetration, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UAttackerAttributeSet, ResistancePenetration, COND_None, REPNOTIFY_Always);
}

void UAttackerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetCriticalHitChanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, 1.0f);
	}
	else if(Attribute == GetResistancePenetrationAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, 1.0f);
	}
	else if(Attribute == GetShieldPenetrationAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, 1.0f);
	}
}

void UAttackerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetCriticalHitChanceAttribute())
	{
		SetCriticalHitChance(FMath::Clamp(GetCriticalHitChance(), 0.0f, 1.0f));
	}
	else if(Data.EvaluatedData.Attribute == GetResistancePenetrationAttribute())
	{
		SetResistancePenetration(FMath::Clamp(GetResistancePenetration(), 0.0f, 1.0f));
	}
	else if(Data.EvaluatedData.Attribute == GetShieldPenetrationAttribute())
	{
		SetShieldPenetration(FMath::Clamp(GetShieldPenetration(), 0.0f, 1.0f));
	}
}

void UAttackerAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttackerAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAttackerAttributeSet::OnRep_ShieldPenetration(const FGameplayAttributeData& OldShieldPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttackerAttributeSet, ShieldPenetration, OldShieldPenetration);
}

void UAttackerAttributeSet::OnRep_ResistancePenetration(const FGameplayAttributeData& OldResistancePenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttackerAttributeSet, ResistancePenetration, OldResistancePenetration);
}
