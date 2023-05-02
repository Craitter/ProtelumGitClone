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

	virtual void PostInitializeComponents() override;

	UPROPERTY(BlueprintAssignable)
	FProjectileLifeEnds OnProjectileLifeEnds;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	virtual void OnRep_ProjectileColor();
	
	virtual void PostNetReceiveVelocity(const FVector& NewVelocity) override;
	
	// AffectionRadius = 0 makes a SingleTargetProjectile
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
	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void ConfirmByInstigator();
	
	UFUNCTION(BlueprintCallable)
	UProjectileMovementComponent* GetProjectileMovement();

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetSkeletalMeshComponent();

	UFUNCTION()
	void OnHitCallback(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void SetLifeSpan(float InLifespan) override;
	
	virtual void LifeSpanExpired() override;

	
	virtual void CallbackLifeSpawnEnds(const FVector& ActorLocation, bool bAlreadyTriggered);
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent = {nullptr};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh = {nullptr};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraSystem = {nullptr};
	
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ProjectileColor)
	FLinearColor ProjectileColor;

	void Explode() const;
	
	void SingleTargetHit(TWeakObjectPtr<UAbilitySystemComponent> TargetASC) const;
	

private:

	void ApplyEffectsAfterHit(const TWeakObjectPtr<UAbilitySystemComponent> LocalTargetASC) const;
	
	static void ApplyEffectsToASC(const TWeakObjectPtr<UAbilitySystemComponent> ASC, const TArray<FGameplayEffectSpecHandle>& GameplayEffectsToApply);

	void SetProjectileColor();

	void SetProjectileVelocity() const;

	void ActivateCollisions();

	void HandleCollision(TWeakObjectPtr<AActor> CollisionActor, const FHitResult& HitResult);

	TArray<FGameplayEffectSpecHandle> EffectsToApplyToTarget;

	TArray<FGameplayEffectSpecHandle> EffectsToApplyToInstigator;

	TObjectPtr<UAbilitySystemComponent> SourceASC = {nullptr};

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = {nullptr};

	float AffectionRadius;
	
	float ProjectileLifeSpan;
	
	FVector VelocityInLocalSpace;
	
	bool bSpawnOnGround;

	bool bDebug = false;

	void DestroyProjectile();

	virtual void PreDestroyFromReplication() override;

	EProjectileType ProjectileType;

};
