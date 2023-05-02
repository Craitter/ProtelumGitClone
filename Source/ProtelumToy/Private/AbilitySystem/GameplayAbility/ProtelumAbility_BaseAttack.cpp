// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/ProtelumAbility_BaseAttack.h"

#include "ProtelumBaseClasses/ProtelumCharacter.h"

UProtelumAbility_BaseAttack::UProtelumAbility_BaseAttack()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

void UProtelumAbility_BaseAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	const TWeakObjectPtr<AProtelumCharacter> Character = Cast<AProtelumCharacter>(ActorInfo->AvatarActor);
	if(Character.IsValid())
	{
		Character->ActivateCurrentWeapon();
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}