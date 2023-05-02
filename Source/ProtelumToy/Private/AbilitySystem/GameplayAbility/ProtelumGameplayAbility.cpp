// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/ProtelumGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "ProtelumBaseClasses/ProtelumPlayerController.h"

UProtelumGameplayAbility::UProtelumGameplayAbility()
{
	//This is Recommended to be false always, if thinking about Activation look in Traneks Documentation
	bServerRespectsRemoteAbilityCancellation = false;
	bHasToBeBoundToInput = true;
}

void UProtelumGameplayAbility::PreActivate(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	// if(bCharacterShouldAim)
	// {
	// 	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Aiming"));
	// }
	//Todo: Make this a GameplayEffect for more flexibility
	switch (CharacterShouldAimWhenAbilityIsActive)
	{	case EAimBehavior::AB_NoAim:
			break;
		case EAimBehavior::AB_Short:
			{
				const FGameplayTag ShortAimTag = FGameplayTag::RequestGameplayTag("State.Aiming.Short");
				ActivationOwnedTags.AddTag(ShortAimTag);
				break;
			}
		case EAimBehavior::AB_Long:
			{
				const FGameplayTag LongAimTag = FGameplayTag::RequestGameplayTag("State.Aiming.Long");
				ActivationOwnedTags.AddTag(LongAimTag);
				break;
			}
		default: ;
	}
		
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UProtelumGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UProtelumGameplayAbility::OnWaitingForConfirmInputBegin()
{
	Super::OnWaitingForConfirmInputBegin();
	const TWeakObjectPtr<APawn> Pawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	if(Pawn.IsValid())
	{
		Controller = Cast<AProtelumPlayerController>(Pawn->GetController());
		if(IsValid(Controller))
		{
			Controller->OnWaitConfirmInputBegin();
		}
	}
}

void UProtelumGameplayAbility::OnWaitingForConfirmInputEnd()
{
	Super::OnWaitingForConfirmInputEnd();
	
	if(IsValid(Controller))
	{
		Controller->OnWaitConfirmInputEnd();
	}
}

const FGameplayTagContainer* UProtelumGameplayAbility::GetCooldownTags() const
{
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	MutableTags->Reset(); // MutableTags writes to the TempCooldownTags on the CDO so clear it in case the ability cooldown tags change (moved to a different slot)
	if (const FGameplayTagContainer* ParentTags = Super::GetCooldownTags())
	{
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	return MutableTags;
}

void UProtelumGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	const TObjectPtr<UGameplayEffect> CooldownGE = GetCooldownGameplayEffect();
	if (IsValid(CooldownGE))
	{
		const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		FActiveGameplayEffectHandle EffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}

FGameplayAbilityTargetingLocationInfo UProtelumGameplayAbility::GetTargetLocationInfoByIndexOfTargetDataHandle(
	FGameplayAbilityTargetDataHandle TargetDataHandle, const int32 Index)
{
	// NOTE, there is two versions of this '::Get(int32 Index)' function; 
	// 1) const version that returns 'const FGameplayAbilityTargetData*', good for reading target data values 
	// 2) non-const version that returns 'FGameplayAbilityTargetData*', good for modifying target data values
	FGameplayAbilityTargetData* Data = TargetDataHandle.Get(Index); // This will valid check the index for you

	
	if(Data == nullptr)
	{
		return FGameplayAbilityTargetingLocationInfo();
	}

	// This is basically the type checking pass, static_cast does not have type safety, this is why we do this check.
	// If we don't do this then it will object slice the struct and thus we have no way of making sure its that type.
	if(Data->GetScriptStruct() == FGameplayAbilityTargetData_LocationInfo::StaticStruct())
	{
		// Here is when you would do the cast because we know its the correct type already
		FGameplayAbilityTargetData_LocationInfo* CustomData = static_cast<FGameplayAbilityTargetData_LocationInfo*>(Data);    
		// UE_LOG(LogTemp, Warning , TEXT("%s() Transform %s"), *FString(__FUNCTION__), *CustomData->TargetLocation.LiteralTransform.ToString());
		return CustomData->TargetLocation;
	}
	return FGameplayAbilityTargetingLocationInfo();
}
