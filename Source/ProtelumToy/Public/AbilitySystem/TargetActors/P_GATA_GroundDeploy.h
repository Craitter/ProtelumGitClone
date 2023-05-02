// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "P_GATA_GroundDeploy.generated.h"

UENUM()
enum class EDeployTargetActorShape : uint8
{
	Capsule				UMETA(DisplayName = "Capsule"),

	Box					UMETA(DisplayName = "Box"),

	Sphere				UMETA(DisplayName = "Sphere"),
};
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API AP_GATA_GroundDeploy : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	AP_GATA_GroundDeploy();
	
	UFUNCTION(BlueprintCallable)
	void Configure(
	UPARAM(DisplayName = "StartLocation") const FGameplayAbilityTargetingLocationInfo& InStartLocation,
	UPARAM(DisplayName = "TraceProfile") FCollisionProfileName InTraceProfile,
	UPARAM(DisplayName = "ActorShape") EDeployTargetActorShape InActorShape,
	UPARAM(DisplayName = "RotationToPlayer") FRotator InRotationToPlayer,
	// UPARAM(DisplayName = "Reticle Class") TSubclassOf<AGameplayAbilityWorldReticle> InReticleClass,
	// UPARAM(DisplayName = "Reticle Params") FWorldReticleParameters InReticleParams,
	UPARAM(DisplayName = "Length") float InLength,
	UPARAM(DisplayName = "Height") float InHeight,
	UPARAM(DisplayName = "Width") float InWidth,
	UPARAM(DisplayName = "Radius") float InRadius,
	UPARAM(DisplayName = "MaxVerticalRange") float InMaxVerticalRange,
	UPARAM(DisplayName = "MaxHorizontalRange") float InMaxHorizontalRange,
	UPARAM(DisplayName = "ShouldProduceTargetDataOnServer") bool bInShouldProduceTargetDataOnServer,
	UPARAM(DisplayName = "RequiresAiming") bool bInRequiresAiming,
	UPARAM(DisplayName = "TraceOriginShouldBeCameraXY") bool bInTraceOriginShouldBeCameraXY,
	UPARAM(DisplayName = "Debug") bool bInDebug
	);
	
	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	virtual void CancelTargeting() override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	
	virtual void Tick(float DeltaSeconds) override;

	virtual void StopTargeting();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh = {nullptr};
protected:
	

	FGameplayAbilityTargetingLocationInfo PerformGroundDeployTrace(TObjectPtr<AActor> InSourceActor);

	void UpdateTargetActorTransform(const FTransform& Transform);

	virtual bool IsConfirmTargetingAllowed() override;

	static FGameplayAbilityTargetDataHandle MakeTargetData(const FGameplayAbilityTargetingLocationInfo& LocationInfo);
	
	float GroundOffset = 0.0f;

	float WallDistance = 0.0f;

	float Height = 0.0f;

	float Length = 0.0f;

	float Width = 0.0f;

	float Radius = 0.0f;

	float MaxVerticalRange = 0.0f;

	float MaxHorizontalRange = 0.0f;
	
	bool bRequiresAiming = true;
	bool bDebug = false;

	bool bTraceOriginShouldBeCameraXY = false;

	EDeployTargetActorShape ActorShape;

	FCollisionProfileName TraceProfile;

	FRotator RotationToPlayer = FRotator::ZeroRotator;

private:
	bool bFirstTick = true;
	bool bWasLastTraceValid = false;

	FGameplayAbilityTargetingLocationInfo TargetingLocationInfo;

	void DetermineSpawnParameters();

	void DetermineCapsuleParameters();
	
	void DetermineBoxParameters();

	void DetermineSphereParameters();

	FTransform PerformHorizontalTrace(FVector& TraceStartLocation, FVector& TraceDirection, FVector& TraceEndLocation, FCollisionQueryParams
	                                  Params) const;
	void PerformSimpleTrace(FHitResult&, const FVector& StartLocation, const FVector& EndLocation, const FName& ProfileName, const
	                        FCollisionQueryParams& Params, const TWeakObjectPtr<UWorld> World) const;
};






