// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/StaticActorAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "AbilitySystem/AbilityActors/DeployActor.h"
#include "Net/UnrealNetwork.h"

void UStaticActorAttributeSet::OnRep_LifeTime(const FGameplayAttributeData& OldLifeTime)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStaticActorAttributeSet, LifeTime, OldLifeTime);
}

void UStaticActorAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
	const FGameplayAttribute& AffectedAttributeProperty) const
{
	const TWeakObjectPtr<UAbilitySystemComponent> AbilitySystem = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if(!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilitySystem.IsValid())
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue(); // SICK WAY OF DOING PERCENTAGE
		const float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;
		AbilitySystem->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UStaticActorAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UStaticActorAttributeSet, LifeTime, COND_None, REPNOTIFY_Always);
}

void UStaticActorAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UStaticActorAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	TWeakObjectPtr<AActor> TargetActor = {nullptr};
	TWeakObjectPtr<ADeployActor> DeployActor = {nullptr};
	//TWeakObjectPtr<AActor> TargetController = {nullptr}; Maybe needed not sure (Works with AI!!!)
	
	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) //Doc says AvatarActor but why?
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		if(TargetActor.IsValid())
		{
			DeployActor = Cast<ADeployActor>(TargetActor);
		}
	}
	// if(Data.EvaluatedData.Attribute == GetLifeTimeAttribute())
	// {
	// 	SetLifeTime(GetLifeTime());
	// }
}
