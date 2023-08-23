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

void UProtelumAbilitySystemComponent::AbilityLocalInputPressed(int32 InputID)
{
	// Consume the input if this InputID is overloaded with GenericConfirm/Cancel and the GenericConfim/Cancel callback is bound
	if (IsGenericConfirmInputBound(InputID))
	{
		LocalInputConfirm();
		return;
	}

	if (IsGenericCancelInputBound(InputID))
	{
		LocalInputCancel();
		return;
	}

	// ---------------------------------------------------------

	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.InputID == InputID)
		{
			if (Spec.Ability)
			{
				Spec.InputPressed = true;
				if (Spec.IsActive())
				{
					if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
					{
						ServerSetInputPressed(Spec.Handle);
					}

					AbilitySpecInputPressed(Spec);

					// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
				}
				else
				{
					TWeakObjectPtr<UProtelumGameplayAbility> InAbility = Cast<UProtelumGameplayAbility>(Spec.Ability);
					if (InAbility.IsValid() && InAbility->ShouldActivateAbilityOnInputPressed())
					{
						// Ability is not active, so try to activate it
						TryActivateAbility(Spec.Handle);
					}
				}
			}
		}
	}
}

void UProtelumAbilitySystemComponent::SetIsPlayerControlled()
{
	AddReplicatedLooseGameplayTag(PlayerControlledTag);
}
