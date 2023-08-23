// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "SimpleProjectile.generated.h"

class AAreaOfEffectActor;
class UProtelumAbilitySystemComponent;
class UProtelumGameplayAbility;
class UGameplayAbility;
class UGameplayEffect;
class UNiagaraComponent;
class UProjectileMovementComponent;

UENUM()
enum class EProjectileType : uint8
{
	Explosive					UMETA(DisplayName = "ExplosiveProjectile"),

	SingleTarget				UMETA(DisplayName = "SingleTargetProjectile"),
	
	PenetrateNonMassive			UMETA(DisplayName = "PenetrateNonMassiveProjectile"),

	// PenetrateMassive			UMETA(DisplayName = "PenetrateMassiveProjectile"),

	// DeployArea				UMETA(DisplayName = "DeployAreaOnGroundProjectile"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProjectileLifeEnds, const FVector&, ActorLocation, bool, bAlreadyTriggered);

UCLASS()
class PROTELUMTOY_API ASimpleProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpleProjectile();
	
	//this event gets Fired after LifeSpawn Ends, On Collision or when the InstigatorConfirms
	UPROPERTY(BlueprintAssignable)
	FProjectileLifeEnds OnProjectileLifeEnds;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnRep_ProjectileColor();

	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	//This Updates the Velocity of the Projectile on Clients (Careful with direction changes)
	virtual void PostNetReceiveVelocity(const FVector& NewVelocity) override;

	UFUNCTION()
	void OnHitCallback(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// this can be called right after spawn and takes care of all Parameters to be applied
	UFUNCTION(BlueprintCallable)
	void ConfigureProjectile
	(
		UPARAM(DisplayName = "ProjectileType")			const EProjectileType InProjectileType,
		UPARAM(DisplayName = "InstigatorEffects")		const TArray<FGameplayEffectSpecHandle>& InInstigatorEffects,
		UPARAM(DisplayName = "TargetEffects")			const TArray<FGameplayEffectSpecHandle>& InTargetEffects,
		UPARAM(DisplayName = "AffectionRadius")			const float InAffectionRadius = 0.0f,
		UPARAM(DisplayName = "ProjectileLifeSpan")		const float InProjectileLifeSpan = 30.0f,
		UPARAM(DisplayName = "ProjectileColor")			const FLinearColor& InProjectileColor = FLinearColor::Blue,
		UPARAM(DisplayName = "VelocityInLocalSpace")	const FVector& InVelocityInLocalSpace = FVector::ZeroVector,
		UPARAM(DisplayName = "Debug")					const bool InDebug = false
	);
	
	UFUNCTION(BlueprintCallable)
	void ConfirmByInstigator();
	
	virtual void SetLifeSpan(float InLifespan) override;
	
	virtual void LifeSpanExpired() override;

	UFUNCTION()
	virtual void CallbackLifeSpawnEnds(const FVector& ActorLocation, bool bAlreadyTriggered);

	UFUNCTION(BlueprintCallable)
	UProjectileMovementComponent* GetProjectileMovement();

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetSkeletalMeshComponent();
protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraSystem = {nullptr};

	//The Color has to be Replicated and Sets the Material on Clients
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ProjectileColor)
	FLinearColor ProjectileColor;
	
	//This gets Called if the Projectile is a Explosive Projectile See EProjectileType
	void Explode() const;
	
	void SingleTargetHit(TWeakObjectPtr<UAbilitySystemComponent> TargetASC) const;

private:
	//These Functions will be Called if there is a valid Target
	void ApplyEffectsAfterHit(const TWeakObjectPtr<UAbilitySystemComponent> LocalTargetASC) const;
	static void ApplyEffectsToASC(const TWeakObjectPtr<UAbilitySystemComponent> ASC, const TArray<FGameplayEffectSpecHandle>& GameplayEffectsToApply);
	
	//Will eventually be called on Hit or Overlap Last Resort on Confirm or Lifetime ends
	void HandleCollision(TWeakObjectPtr<AActor> CollisionActor, const FHitResult& HitResult);
	
	//Calls Destroy at the End and before we can handle things like Particles or Sounds
	void DestroyProjectile(bool NetForce);
	
	//Calls DestroyProjectile before being Destroyed
	virtual void PreDestroyFromReplication() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent = {nullptr};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh = {nullptr};

	//Effects that will be Applied to every Target
	TArray<FGameplayEffectSpecHandle> EffectsToApplyToTarget;

	//Effects that will be Applied to Instigator for every Target Hit
	TArray<FGameplayEffectSpecHandle> EffectsToApplyToInstigator;

	//This is Set on Begin play and stays as reference for Instigator Effects
	TObjectPtr<UAbilitySystemComponent> SourceASC = {nullptr};

	//For changes to Material depending on the Ability which is casted
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = {nullptr};

	//Begin Blueprint defined Values
	float AffectionRadius;
	
	float ProjectileLifeSpan;
	
	FVector VelocityInLocalSpace;
	
	bool bSpawnOnGround;

	bool bDebug = false;

	EProjectileType ProjectileType;
	//End Blueprint defined Values

	void SetProjectileColor();

	void SetProjectileVelocity() const;

	void ActivateCollisions();
};
