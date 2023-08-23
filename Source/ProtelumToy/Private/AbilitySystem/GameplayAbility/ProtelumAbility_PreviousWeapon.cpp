// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/ProtelumAbility_PreviousWeapon.h"

#include "ProtelumBaseClasses/ProtelumCharacter.h"

UProtelumAbility_PreviousWeapon::UProtelumAbility_PreviousWeapon()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

void UProtelumAbility_PreviousWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			return;
		}
	
		const TWeakObjectPtr<AProtelumCharacter> Character = Cast<AProtelumCharacter>(ActorInfo->AvatarActor);
		if(Character.IsValid())
		{
			Character->PreviousWeapon();
		}
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}
