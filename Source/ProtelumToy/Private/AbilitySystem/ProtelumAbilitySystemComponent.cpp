// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ProtelumAbilitySystemComponent.h"

#include "AbilitySystem/GameplayAbility/ProtelumGameplayAbility.h"
#include "AbilitySystem/Interface/AbilityBindingInterface.h"


UProtelumAbilitySystemComponent::UProtelumAbilitySystemComponent()
{
	PlayerControlledTag	= FGameplayTag::RequestGameplayTag("PlayerControlled");
}

void UProtelumAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	//Avatar Actor Interface Call to Bind Abilities
	// TObjectPtr<APawn> Pawn = Cast<APawn>(GetAvatarActor_Direct());
	// if(IsValid(Pawn) && Pawn->IsPlayerControlled())
	// {
	bool bHasBinding = true;
	const TWeakObjectPtr<UProtelumGameplayAbility> Ability = Cast<UProtelumGameplayAbility>(AbilitySpec.Ability);
	if(Ability.IsValid())
	{
		bHasBinding = Ability->ShouldAbilityBindToInput();
	}
	if(bHasBinding)
	{
		if(IAbilityBindingInterface* ABI = Cast<IAbilityBindingInterface>(GetAvatarActor_Direct()))
		{
			ABI->BindAbility(AbilitySpec);
		}	
	}
	// }
	Super::OnGiveAbility(AbilitySpec);
}

void UProtelumAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	//Avatar Actor Interface Call to UnBind Abilities
	if(AbilityActorInfo.IsValid())
	{
		if(IAbilityBindingInterface* ABI = Cast<IAbilityBindingInterface>(AbilityActorInfo->AvatarActor.Get()))
		{
			ABI->UnbindAbility(AbilitySpec);
		}
	}
	
	Super::OnRemoveAbility(AbilitySpec);
}

void UProtelumAbilitySystemComponent::SetIsPlayerControlled()
{
	AddReplicatedLooseGameplayTag(PlayerControlledTag);
}
