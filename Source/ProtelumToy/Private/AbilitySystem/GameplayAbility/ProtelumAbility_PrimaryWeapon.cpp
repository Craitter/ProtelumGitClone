// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\Public\AbilitySystem\GameplayAbility\ProtelumAbility_PrimaryWeapon.h"

#include "ProtelumBaseClasses/ProtelumCharacter.h"

UProtelumAbility_PrimaryWeapon::UProtelumAbility_PrimaryWeapon()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

void UProtelumAbility_PrimaryWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
			Character->PrimaryWeapon();
		}
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}
