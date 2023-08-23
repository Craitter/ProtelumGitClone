// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Interface/ProtelumInteractable.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_NetworkSyncPoint.h"

// Add default functionality here for any IProtelumInteractable functions that are not pure virtual.
bool IProtelumInteractable::IsAvailableForInteraction_Implementation(UPrimitiveComponent* InteractionComponent) const
{
	return false;
}

FInteractMessageInformation IProtelumInteractable::GetInteractionMessageType_Implementation(
	UPrimitiveComponent* InteractionComponent) const
{
	return FInteractMessageInformation();
}

float IProtelumInteractable::GetInteractionDuration_Implementation(UPrimitiveComponent* InteractionComponent) const
{
	return 0.0f;
}

void IProtelumInteractable::GetPreInteractSyncType_Implementation(bool& bShouldSync, EAbilityTaskNetSyncType& Type,
	UPrimitiveComponent* InteractionComponent) const
{
	bShouldSync = false;
	Type = EAbilityTaskNetSyncType::OnlyServerWait;
}

void IProtelumInteractable::GetPostInteractSyncType_Implementation(bool& bShouldSync, EAbilityTaskNetSyncType& Type,
	UPrimitiveComponent* InteractionComponent) const
{
	bShouldSync = false;
	Type = EAbilityTaskNetSyncType::OnlyServerWait;
}

FSimpleMulticastDelegate* IProtelumInteractable::GetTargetCancelInteractionDelegate(
	UPrimitiveComponent* InteractionComponent)
{
	return nullptr;
}

void IProtelumInteractable::RegisterInteracter_Implementation(UPrimitiveComponent* InteractionComponent,
	AActor* InteractingActor)
{
	if (Interacters.Contains(InteractionComponent))
	{
		TArray<TWeakObjectPtr<AActor>>& InteractingActors = Interacters[InteractionComponent];
		if (!InteractingActors.Contains(InteractingActor))
		{
			InteractingActors.Add(InteractingActor);
		}
	}
	else
	{
		TArray<TWeakObjectPtr<AActor>> InteractingActors;
		InteractingActors.Add(InteractingActor);
		Interacters.Add(InteractionComponent, InteractingActors);
	}
}

void IProtelumInteractable::UnregisterInteracter_Implementation(UPrimitiveComponent* InteractionComponent,
	AActor* InteractingActor)
{
		if (Interacters.Contains(InteractionComponent))
		{
			TArray<TWeakObjectPtr<AActor>>& InteractingActors = Interacters[InteractionComponent];
			InteractingActors.Remove(InteractingActor);
		}
}

void IProtelumInteractable::InteractableCancelInteraction_Implementation(UPrimitiveComponent* InteractionComponent)
{
	if (Interacters.Contains(InteractionComponent))
	{
		FGameplayTagContainer InteractAbilityTagContainer;
		InteractAbilityTagContainer.AddTag(FGameplayTag::RequestGameplayTag("State.Inter"));

		TArray<TWeakObjectPtr<AActor>>& InteractingActors = Interacters[InteractionComponent];
		for (TWeakObjectPtr<AActor> InteractingActor : InteractingActors)
		{
			TWeakObjectPtr<UAbilitySystemComponent> ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InteractingActor.Get());

			if (ASC.IsValid())
			{
				ASC->CancelAbilities(&InteractAbilityTagContainer);
			}
		}
		InteractingActors.Empty();
	}
}
