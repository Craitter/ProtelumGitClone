// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/ProtelumAbility_ShoulderSwitch.h"

#include "ProtelumBaseClasses/ProtelumCharacter.h"


UProtelumAbility_ShoulderSwitch::UProtelumAbility_ShoulderSwitch()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

void UProtelumAbility_ShoulderSwitch::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const TWeakObjectPtr<AProtelumCharacter> Character = Cast<AProtelumCharacter>(ActorInfo->AvatarActor);
	if(Character.IsValid())
	{
		const TWeakObjectPtr<UProtelumSpringArmComponent> SpringArmComponent = Character->GetSpringArm();
		if(SpringArmComponent.IsValid())
		{
			SpringArmComponent->OffsetSet.BindUObject(this, &UProtelumAbility_ShoulderSwitch::EndAbility, Handle, ActorInfo, ActivationInfo, true, false);
			SpringArmComponent->SwitchSpringArmShoulder();
		}
	}
}

void UProtelumAbility_ShoulderSwitch::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}
