// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "P_AT_WaitTargetDataUsingActor.generated.h"


class AGameplayAbilityTargetActor;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitTargetDataUsingActorDelegate, const FGameplayAbilityTargetDataHandle&,
                                            Data);
/**
 * Waits for TargetData from an already spawned TargetActor and does *NOT* destroy it when it receives data.
 *
 * The original WaitTargetData's comments expects us to subclass it heavily, but the majority of its functions
 * are not virtual. Therefore this is a total rewrite of it to add bCreateKeyIfNotValidForMorePredicting functionality.
 */
UCLASS()
class PROTELUMTOY_API UP_AT_WaitTargetDataUsingActor : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	/**
	* Uses specified spawned TargetActor and waits for it to return valid data or to be canceled. The TargetActor is *NOT* destroyed.
	*
	* @param OwningAbility
	* @param TaskInstanceName
	* @param InConfirmationType
	* @param InTargetActor
	* @param bCreateKeyIfNotValidForMorePredicting Will create a new scoped prediction key if the current scoped prediction key is not valid for more predicting.
	* If false, it will always create a new scoped prediction key. We would want to set this to true if we want to use a potentially existing valid scoped prediction
	* key like the ability's activation key in a batched ability.
	*/

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParms = "Instigator", Category = "Ability|Tasks"))
	static UP_AT_WaitTargetDataUsingActor* WaitTargetDataReUsingActor(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		TEnumAsByte<EGameplayTargetingConfirmation::Type> InConfirmationType,
		AGameplayAbilityTargetActor* InTargetActor,
		bool bCreateKeyIfNotValidForMorePredicting = false);

	virtual void Activate() override;

	/** Called when the ability is asked to confirm from an outside node. What this means depends on the individual task. By default, this does nothing other than ending if bEndTask is true. */
	virtual void ExternalConfirm(bool bEndTask) override;

	/** Called when the ability is asked to cancel from an outside node. What this means depends on the individual task. By default, this does nothing other than ending the task. */
	virtual void ExternalCancel() override;

	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataUsingActorDelegate ValidData;

	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataUsingActorDelegate Cancelled;

	UFUNCTION()
	virtual void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag);

	UFUNCTION()
	virtual void OnTargetDataReplicatedCancelledCallback();

	UFUNCTION()
	virtual void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	virtual void OnTargetDataCancelledCallback(const FGameplayAbilityTargetDataHandle& Data);

protected:

	TObjectPtr<AGameplayAbilityTargetActor> TargetActor;

	bool bCreateKeyIfNotValidForMorePredicting;

	TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType;

	FDelegateHandle OnTargetDataReplicatedCallbackDelegateHandle;

	virtual void InitializeTargetActor() const;
	virtual void FinalizeTargetActor() const;

	virtual void RegisterTargetDataCallbacks();

	virtual void OnDestroy(bool AbilityEnded) override;

	virtual bool ShouldReplicateDataToServer() const;
};
