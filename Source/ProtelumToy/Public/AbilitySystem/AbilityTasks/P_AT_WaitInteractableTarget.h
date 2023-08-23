// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "P_AT_WaitInteractableTarget.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitInteractableTargetDelegate, const FGameplayAbilityTargetDataHandle&, Data);

/**
 * Performs a line trace on a timer, looking for an Actor that implements IGSInteractable that is available for interaction.
 * The StartLocations are hardcoded for GASShooter since we can be in first and third person so we have to check every time
 * we trace. If you only have one start location, you should make it more generic with a parameter on your AbilityTask node.
 */
UCLASS()
class PROTELUMTOY_API UP_AT_WaitInteractableTarget : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FWaitInteractableTargetDelegate FoundNewInteractableTarget;

	UPROPERTY(BlueprintAssignable)
	FWaitInteractableTargetDelegate LostInteractableTarget;

	/**
	* Traces a line on a timer looking for InteractableTargets.
	* @param MaxRange How far to trace.
	* @param TimerPeriod Period of trace timer.
	* @param bShowDebug Draws debug lines for traces.
	*/
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParms = "Instigator"), Category = "Ability|Tasks")
	static UP_AT_WaitInteractableTarget* WaitForInteractableTarget(UGameplayAbility* OwningAbility, FName TaskInstanceName, FGameplayAbilityTargetingLocationInfo StartLocation, FCollisionProfileName TraceProfile, float MaxRange = 200.0f, float TimerPeriod = 0.1f, bool bShowDebug = true);

	virtual void Activate() override;

protected:

	FGameplayAbilityTargetingLocationInfo StartLocation;
	
	float MaxRange;

	float TimerPeriod;

	bool bShowDebug;

	FCollisionProfileName TraceProfile;

	FGameplayAbilityTargetDataHandle TargetData;

	FTimerHandle TraceTimerHandle;

	virtual void OnDestroy(bool AbilityEnded) override;


	/** Traces as normal, but will manually filter all hit actors */
	void LineTrace(FHitResult& OutHitResult, const UWorld* World, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params) const;

	bool GetInteractable(const TArray<FHitResult>& InHitResults, FHitResult& OutHitResult) const;

	void GetInteractionTraceStartAndEndLocation(FVector& Start, FVector& End, const UWorld* World, FName ProfileName, const FCollisionQueryParams& Params) const;

	UFUNCTION()
	void PerformTrace();

	FGameplayAbilityTargetDataHandle MakeTargetData(const FHitResult& HitResult) const;
	
};
