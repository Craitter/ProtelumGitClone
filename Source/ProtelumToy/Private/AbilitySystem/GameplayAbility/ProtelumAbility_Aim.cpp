// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/ProtelumAbility_Aim.h"


#include "AbilitySystemComponent.h"
#include "ProtelumBaseClasses/ProtelumCharacter.h"

UProtelumAbility_Aim::UProtelumAbility_Aim()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ShortAimTag = FGameplayTag::RequestGameplayTag("State.Aiming.Short");
	LongAimTag = FGameplayTag::RequestGameplayTag("State.Aiming.Long");
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = ShortAimTag;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagPresent;
	AbilityTriggers.Add(TriggerData);
	TriggerData.TriggerTag = LongAimTag;
	AbilityTriggers.Add(TriggerData);
	
	bHasToBeBoundToInput = false;
	bServerRespectsRemoteAbilityCancellation = true;
}

void UProtelumAbility_Aim::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			return;
		}
	
		Character = Cast<AProtelumCharacter>(GetAvatarActorFromActorInfo());
		if(IsValid(Character))
		{
			// UE_LOG(LogTemp, Warning , TEXT("%s Trigger %s"), *FString(__FUNCTION__), *TriggerEventData->EventTag.ToString());
			if(LongAimTag == TriggerEventData->EventTag)
			{
				bDelayAimEnd = true;
			}
			Character->SetMState(EMovementState::MS_Aim);
			bOutReplicateEndAbility = true;
		}
	}
}

void UProtelumAbility_Aim::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	const TWeakObjectPtr<UAbilitySystemComponent> ASC = GetAbilitySystemComponentFromActorInfo();
	if(ASC.IsValid())
	{
		if(ASC->HasMatchingGameplayTag(LongAimTag))
		{
			bDelayAimEnd = true;
			return;
		}
		if(ASC->HasMatchingGameplayTag(ShortAimTag))
		{
			bDelayAimEnd = false;
			return;
		}
	}
	
	if(bDelayAimEnd)
	{
		OutHandle = Handle;
		OutActivationInfo = ActivationInfo;
		bOutReplicateEndAbility = bReplicateEndAbility;
		bOutWasCancelled = bWasCancelled;
		const TWeakObjectPtr<UWorld> World = GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.SetTimer(DelayAimTimer, this, &UProtelumAbility_Aim::OnAimEnd, TimeUntilAimingEnds);
		bDelayAimEnd = true;
		return;
	}
	if(IsValid(Character))
	{
		Character->SetMState(EMovementState::MS_Walk);
	}	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UProtelumAbility_Aim::OnAimEnd()
{
	bDelayAimEnd = false;
	EndAbility(OutHandle, CurrentActorInfo, OutActivationInfo, bOutReplicateEndAbility, bOutWasCancelled);
}
