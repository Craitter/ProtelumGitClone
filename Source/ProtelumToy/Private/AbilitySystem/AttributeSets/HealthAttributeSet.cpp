// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "ProtelumBaseClasses/ProtelumCharacterBase.h"

void UHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, HealthRegenerationRate, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, ShieldRegenerationRate, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, DamageResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, DamageVulnerability, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);	
}

void UHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health,MaxHealth, NewValue, GetHealthAttribute());
	}

	else if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	
	else if(Attribute == GetShieldAttribute())
	{
		if(NewValue <= 0.0f)
		{
			NewValue = 0.0f;
		}
	}
	
	else if(Attribute == GetDamageResistanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, 1.0f);
	}
	
	else if(Attribute == GetDamageVulnerabilityAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, 1.0f);
	}
	
	else if(Attribute == GetCriticalHitResistanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, 1.0f);
	}
}

void UHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	
	else if(Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		if(GetShield() <= 0.0f)
		{
			SetShield(0.0f);
		}
	}
	
	else if(Data.EvaluatedData.Attribute == GetDamageResistanceAttribute())
	{
		SetDamageResistance(FMath::Clamp(GetDamageResistance(), 0.0f, 1.0f));
	}
	
	else if(Data.EvaluatedData.Attribute == GetDamageVulnerabilityAttribute())
	{
		SetDamageVulnerability(FMath::Clamp(GetDamageVulnerability(), 0.0f, 1.0f));
	}
	
	else if(Data.EvaluatedData.Attribute == GetCriticalHitResistanceAttribute())
	{
		SetCriticalHitResistance(FMath::Clamp(GetCriticalHitResistance(), 0.0f, 1.0f));
	}
	
	else if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float LocalDamage = GetDamage();
		SetDamage(0.0f);
		if(LocalDamage >= 0.0f)
		{
			SetHealth(GetHealth() - LocalDamage);
		}
	}
	//
	// 	// FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	// // UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	// // const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	// // FGameplayTagContainer SpecAssetTags;
	// // Data.EffectSpec.GetAllAssetTags(SpecAssetTags);
	//
	//
	//
	// //Get the TargetActor, this should be the Owner of THIS set
	// TWeakObjectPtr<AActor> TargetActor = {nullptr};
	//
	// //TWeakObjectPtr<AActor> TargetController = {nullptr}; Maybe needed not sure (Works with AI!!!)
	//
	// TWeakObjectPtr<AProtelumCharacterBase> TargetCharacter = {nullptr};
	// if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) //Doc says AvatarActor but why?
	// {
	// 	TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
	// 	TargetCharacter = Cast<AProtelumCharacterBase>(TargetActor);
	// }
	// //End of Getting TargetActor
	//
	// // Get Source Actor
	//
	// const FGameplayEffectContextHandle EffectContext = Data.EffectSpec.GetContext();
	// const TWeakObjectPtr<UAbilitySystemComponent> SourceAbilitySystem = EffectContext.GetOriginalInstigatorAbilitySystemComponent();
	//
	// TWeakObjectPtr<AActor> SourceActor = {nullptr};
	// TWeakObjectPtr<AController> SourceController = {nullptr}; //For what do we need it atm?
	// TWeakObjectPtr<AProtelumCharacterBase> SourceCharacter = {nullptr}; //NotNeededYet
	//
	// if(SourceAbilitySystem.IsValid() &&
	// 	SourceAbilitySystem->AbilityActorInfo.IsValid() &&
	// 	SourceAbilitySystem->AbilityActorInfo->AvatarActor.IsValid())
	// {
	// 	SourceActor = SourceAbilitySystem->AbilityActorInfo->AvatarActor.Get();
	// 	SourceController = SourceAbilitySystem->AbilityActorInfo->PlayerController.Get();
	// 	//Often Null so we check if we need to try it again with a different approach
	// 	if (SourceController == nullptr && SourceActor != nullptr)	
	// 	{
	// 		if (const APawn* Pawn = Cast<APawn>(SourceActor))
	// 		{
	// 			SourceController = Pawn->GetController();
	// 		}
	// 	}
	// 	
	// 	// Use the controller to find the source pawn if null there is maybe no controller
	// 	if (SourceController.IsValid())
	// 	{
	// 		SourceCharacter = Cast<AProtelumCharacterBase>(SourceController->GetPawn());
	// 	}
	// 	else
	// 	{
	// 		SourceCharacter = Cast<AProtelumCharacterBase>(SourceActor);
	// 	}
	// 	if (EffectContext.GetEffectCauser())
	// 	{
	// 		SourceActor = EffectContext.GetEffectCauser();
	// 	}
	// }
	// // End of Getting Source, now we can do all we want, if ... capability and so on
	//
	// if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	// {
	// 	//handle other Health Changes
	// 	//Health loss should go through Damage
	// 	SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	// }
	// else if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	// {
	// 	// Try to extract a hit result IF NEEDED
	// 	// FHitResult HitResult;
	// 	// if (EffectContext.GetHitResult())
	// 	// {
	// 	// 	HitResult = *EffectContext.GetHitResult();
	// 	// }
	//
	// 	//Since Damage will now be applied we can reset it
	// 	float LocalDamage = GetDamage();
	// 	SetDamage(0.0f);
	//
	// 	if(LocalDamage > 0.0f)
	// 	{
	// 		// If character was alive before damage is added, handle damage
	// 		// This prevents damage being added to dead things and replaying death animations not yet but good point
	// 		bool bIsAlive = true;
	// 		if(TargetCharacter.IsValid())
	// 		{
	// 			bIsAlive = TargetCharacter->IsAlive();
	// 			if(!bIsAlive)
	// 			{
	// 				LocalDamage = 0.0f; //Just an idea
	// 				//Do something if already dead
	// 			}
	// 		}
	// 		const float ShieldAfterDamage = GetShield() - LocalDamage;
	// 		if(ShieldAfterDamage <= 0)
	// 		{
	// 			//InstantGameplayEffect to Set Shield to 0;
	// 			LocalDamage = FMath::Abs(ShieldAfterDamage);
	// 		}
	// 		else
	// 		{
	// 			LocalDamage = 0.0f;
	// 			//InstantGameplayEffet to Set Shield to ShieldAfterDamage
	// 		}
	// 		if(LocalDamage > 0.0f)
	// 		{
	// 			const float NewHealth = GetHealth() - LocalDamage;
	// 			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
	// 		}
	// 		if(!TargetCharacter->IsAlive())
	// 		{
	// 			//Should be handled by GameplayEffects
	// 			// SetGold(0.0f);
	// 			// SetXP(0.0f);
	// 			// SetCharacterLevel(1.0f);
	// 		}
	// 	}
	// }
	
}

// void UHealthAttributeSet::OnAttributeAggregatorCreated(const FGameplayAttribute& Attribute,
// 	FAggregator* NewAggregator) const
// {
// 	Super::OnAttributeAggregatorCreated(Attribute, NewAggregator);
// }

void UHealthAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
                                                      const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
                                                      const FGameplayAttribute& AffectedAttributeProperty) const
{
	const TWeakObjectPtr<UAbilitySystemComponent> AbilitySystem = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if(!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilitySystem.IsValid())
	{
		if(CurrentMaxValue > NewMaxValue)
		{
			//Reduce percentage
			const float PercentageReduction = NewMaxValue / CurrentMaxValue;
			const float NewCurrentValue = AffectedAttribute.GetCurrentValue() * PercentageReduction;
			const float MaxChangeAffectionDelta = NewCurrentValue - AffectedAttribute.GetCurrentValue();
			AbilitySystem->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, MaxChangeAffectionDelta);

			// const float CurrentValue = AffectedAttribute.GetCurrentValue(); // SICK WAY OF DOING PERCENTAGE
			// float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;
		}
		else
		{
			const float AbsDeltaMaxValue = NewMaxValue - CurrentMaxValue;
			AbilitySystem->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, AbsDeltaMaxValue);
		}
	}
}

void UHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHeath)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, Health, OldHeath);
}

void UHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, MaxHealth, OldMaxHealth);
}

void UHealthAttributeSet::OnRep_HealthRegenerationRate(const FGameplayAttributeData& OldHealthRegenerationRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, HealthRegenerationRate, OldHealthRegenerationRate);
}

void UHealthAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, Shield, OldShield);
}

void UHealthAttributeSet::OnRep_DamageResistance(const FGameplayAttributeData& OldDamageResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, DamageResistance, OldDamageResistance);
}

void UHealthAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UHealthAttributeSet::OnRep_DamageVulnerability(const FGameplayAttributeData& OldDamageVulnerability)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, DamageVulnerability, OldDamageVulnerability);
}

void UHealthAttributeSet::OnRep_ShieldRegenerationRate(const FGameplayAttributeData& OldShieldRegenerationRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, ShieldRegenerationRate, OldShieldRegenerationRate);
}
