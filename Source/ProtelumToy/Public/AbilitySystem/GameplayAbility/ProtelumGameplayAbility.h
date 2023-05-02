// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ProtelumGameplayAbility.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAimBehavior : uint8
{
	//The Ability does not rotate the Character to the Camera Rotation
	AB_NoAim = 0		UMETA(DisplayName = "NoAim"),
	//Aim Rotation stops after the Ability is finished
	AB_Short = 1			UMETA(DisplayName = "Fast"),
	//AimRotation lasts for 3 Seconds after Aiming
	AB_Long = 2			UMETA(DisplayName = "Long"),
};

class UProtelumAimGameplayEffect;
class AProtelumPlayerController;
UCLASS()
class PROTELUMTOY_API UProtelumGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UProtelumGameplayAbility();
	
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// //ClientOnly
	virtual void OnWaitingForConfirmInputBegin() override;
	// //ClientOnly
	virtual void OnWaitingForConfirmInputEnd() override;
	
	bool ShouldAbilityBeRemovedOnLevelChange() const { return bLevelChangeRemovesAbility;}

	bool ShouldAbilityBindToInput() const { return bHasToBeBoundToInput;}

	const FScalableFloat& GetCooldownDuration() const { return CooldownDuration;}

	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
protected:
	//Todo: Create Protelum BlueprintLibrary
	UFUNCTION(BlueprintCallable)
	FGameplayAbilityTargetingLocationInfo GetTargetLocationInfoByIndexOfTargetDataHandle(
		FGameplayAbilityTargetDataHandle TargetData, const int32 Index);

	// UPROPERTY(EditDefaultsOnly)
	// bool bCharacterShouldAim;
	
	//this Determines if the Character Should be facing in Camera Direction during this Ability or not
	UPROPERTY(EditDefaultsOnly)
	EAimBehavior CharacterShouldAimWhenAbilityIsActive;

	UPROPERTY(EditDefaultsOnly)
	bool bLevelChangeRemovesAbility;

	UPROPERTY(EditDefaultsOnly)
	bool bHasToBeBoundToInput;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FScalableFloat CooldownDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FGameplayTagContainer CooldownTags;

	// Temp container that we will return the pointer to in GetCooldownTags().
	// This will be a union of our CooldownTags and the Cooldown GE's cooldown tags.
	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;
private:
	TObjectPtr<AProtelumPlayerController> Controller;
};
