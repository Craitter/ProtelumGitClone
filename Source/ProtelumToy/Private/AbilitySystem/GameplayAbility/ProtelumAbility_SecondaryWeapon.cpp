// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\Public\AbilitySystem\GameplayAbility\ProtelumAbility_SecondaryWeapon.h"

#include "ProtelumBaseClasses/ProtelumCharacter.h"

UProtelumAbility_SecondaryWeapon::UProtelumAbility_SecondaryWeapon()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

void UProtelumAbility_SecondaryWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	const TWeakObjectPtr<AProtelumCharacter> Character = Cast<AProtelumCharacter>(ActorInfo->AvatarActor);
	if(Character.IsValid())
	{
		Character->SecondaryWeapon();
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
