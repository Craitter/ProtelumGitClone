// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Calculations/ExecuteCalc/DamageExecuteCalculation.h"

#include "AbilitySystem/ProtelumAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/AttackerAttributeSet.h"
#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"

struct FProtelumDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ShieldPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistancePenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageVulnerability)

	FProtelumDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, Shield, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, DamageResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttackerAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttackerAttributeSet, ShieldPenetration, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttackerAttributeSet, ResistancePenetration, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, Damage, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, DamageVulnerability, Target, true);
	}
};

static const FProtelumDamageStatics& DamageStatics()
{
	static FProtelumDamageStatics DStatics;
	return DStatics;
}

UDamageExecuteCalculation::UDamageExecuteCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().ShieldDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ShieldPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistancePenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageVulnerabilityDef);
}

bool UDamageExecuteCalculation::IsACriticalHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                               const FAggregatorEvaluateParameters& EvaluationParameters)
{
	bool bIsCriticalHit = false;
	float ChanceToBeACriticalHit = 0.0f;
	float AttributeCriticalHitChance;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, AttributeCriticalHitChance);
	ChanceToBeACriticalHit += AttributeCriticalHitChance;
	if ((ChanceToBeACriticalHit > 0.0f + SMALL_NUMBER) && (FMath::FRand() <= ChanceToBeACriticalHit))
	{
		bIsCriticalHit = true;
	}
	if(bIsCriticalHit)
	{
		float CriticalHitResistance = 0.0f; 
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters,CriticalHitResistance);
		if((CriticalHitResistance > 0.0f + SMALL_NUMBER) && (FMath::Rand() <= CriticalHitResistance))
		{
			bIsCriticalHit = false;
		}
	}
	return bIsCriticalHit;
}

void UDamageExecuteCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                       FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const TWeakObjectPtr<UProtelumAbilitySystemComponent> TargetASC = Cast<UProtelumAbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent());
	const TWeakObjectPtr<UProtelumAbilitySystemComponent> SourceASC = Cast<UProtelumAbilitySystemComponent>(ExecutionParams.GetSourceAbilitySystemComponent());

	TWeakObjectPtr<AActor> TargetActor = nullptr;
	if(TargetASC.IsValid())
	{
		TargetActor = TargetASC->GetAvatarActor();
	}

	TWeakObjectPtr<AActor> SourceActor = nullptr;
	if(SourceASC.IsValid())
	{
		SourceActor = SourceASC->GetAvatarActor();
	}

	const FGameplayEffectSpec& GameplayEffectSpec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = GameplayEffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = GameplayEffectSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Damage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);
	// Add SetByCaller damage if it exists
	Damage += FMath::Max<float>(GameplayEffectSpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	if(IsACriticalHit(ExecutionParams, EvaluationParameters))
	{
		Damage *= 2;
		// UE_LOG(LogTemp, Warning , TEXT("CriticalHit"));
	}
	
	float ShieldPenetration = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ShieldPenetrationDef, EvaluationParameters,ShieldPenetration);
	const float UnblockedDamage = Damage * ShieldPenetration;
	
	float Shield = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ShieldDef, EvaluationParameters,Shield);
	
	const float BlockableDamage = Damage - UnblockedDamage;
	float OverwhelmDamage = BlockableDamage - Shield;
	OverwhelmDamage = FMath::Max<float>(OverwhelmDamage, 0.0f);
	Damage = UnblockedDamage + OverwhelmDamage;
	// UE_LOG(LogTemp, Warning , TEXT("Damage AfterShield %f"), Damage);
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().ShieldProperty, EGameplayModOp::Additive, -BlockableDamage));
	
	float DamageResistance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageResistanceDef, EvaluationParameters,DamageResistance);
	float DamagePenetration = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ResistancePenetrationDef, EvaluationParameters,DamagePenetration);

	float DamageVulnerability = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageVulnerabilityDef, EvaluationParameters,DamageVulnerability);
	
	Damage *= (1.0f + DamageVulnerability);
	// UE_LOG(LogTemp, Warning , TEXT("Damage AfterVulnerable %f"), Damage);
	const float RawDamage =  Damage * DamagePenetration;
	Damage -= RawDamage;
	// UE_LOG(LogTemp, Warning , TEXT("RawDamage %f"), RawDamage);
	const float MitigatedDamage = Damage * DamageResistance;
	// UE_LOG(LogTemp, Warning , TEXT("Mitigated Damage %f"), MitigatedDamage);
	Damage = Damage + RawDamage - MitigatedDamage;

	// Set the Target's damage meta attribute
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, Damage));
}
