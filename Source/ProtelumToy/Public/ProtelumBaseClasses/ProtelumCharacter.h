// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "AbilitySystemInterface.h"
#include "Character/Components/ProtelumSpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ProtelumCharacterBase.h"
// #include "AbilitySystem/Interface/AbilityBindingInterface.h"
#include "ProtelumCharacter.generated.h"


class ASimpleProjectile;
class AGameplayAbilityTargetActor;
class AP_GATA_GroundDeploy;
class UNiagaraComponent;
struct FGameplayAbilitySpec;


// USTRUCT(BlueprintType)
// struct FTargetActorInfo
// {
// 	GENERATED_BODY()
// 	
// 	UPROPERTY()
// 	UGameplayAbility* AbilityReference;
//
// 	UPROPERTY()
// 	AGameplayAbilityTargetActor* TargetActor;
// 	
// };

UENUM(BlueprintType)
enum EMovementState
{
	MS_Aim = 0		UMETA(DisplayName = "Aiming"),
	MS_Walk = 1		UMETA(DisplayName = "Walking")
};

UCLASS()
class PROTELUMTOY_API AProtelumCharacter : public AProtelumCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProtelumCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	UProtelumSpringArmComponent* GetSpringArm() const {return SpringArm;}
	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetCamera() const {return Camera;}
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetWeapon() const {return WeaponComponent;}
	UFUNCTION(BlueprintCallable)
	UNiagaraComponent* GetWeaponNiagaraSystem() const {return WeaponNiagaraSystem;}
	UFUNCTION(BlueprintCallable)
	UNiagaraComponent* GetNiagaraSystem() const {return NiagaraSystem;}
	
	UFUNCTION(BlueprintCallable)
	void SetMState(EMovementState NewMState);

	UFUNCTION(BlueprintCallable)
	AGameplayAbilityTargetActor* GetTargetActorReference(UGameplayAbility* ReferencedAbility, TSubclassOf<AGameplayAbilityTargetActor> TargetActorClass);
	UFUNCTION(BlueprintCallable)
	void SetTargetActorReference(UGameplayAbility* ReferencedAbility, AGameplayAbilityTargetActor* ReferencedTargetActor);

	void PrimaryWeapon();
	void SecondaryWeapon();
	void NextWeapon();
	void PreviousWeapon();

	void ActivateCurrentWeapon() const;
protected:
	void SetCurrentWeapon(FGameplayTag WeaponTag);
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PrimaryWeaponTag;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SecondaryWeaponTag;

private:
	// 	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	//The CameraBoom/SpringArm Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProtelumSpringArmComponent> SpringArm = {nullptr};

	//The Camera Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera = {nullptr};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> WeaponComponent = {nullptr};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> WeaponNiagaraSystem = {nullptr};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraSystem = {nullptr};

	// //This Map saves References to TargetActors, so we can reuse them with passing the ability
	// UPROPERTY()
	// TArray<FTargetActorInfo> TargetActorInfos;

	UPROPERTY()
	AGameplayAbilityTargetActor* GameplayAbilityTargetActor = {nullptr};

	EMovementState MState = EMovementState::MS_Walk;
	bool bMStateChanged = false;

	FGameplayTag CurrentWeaponTag;
};
