// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TargetActors/P_GATA_GroundDeploy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "Abilities/GameplayAbility.h"
#include "ProtelumBaseClasses/ProtelumCharacter.h"

AP_GATA_GroundDeploy::AP_GATA_GroundDeploy()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AActor::SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(Mesh);
}

void AP_GATA_GroundDeploy::Configure(const FGameplayAbilityTargetingLocationInfo& InStartLocation,
	FCollisionProfileName InTraceProfile, EDeployTargetActorShape InActorShape, FRotator InRotationToPlayer,
	float InLength, float InHeight, float InWidth, float InRadius, float InMaxVerticalRange, float InMaxHorizontalRange,
	bool bInShouldProduceTargetDataOnServer, bool bInRequiresAiming, bool bInTraceOriginShouldBeCameraXY, bool bInDebug)
{
	StartLocation = InStartLocation;
	TraceProfile = InTraceProfile;
	ActorShape = InActorShape;
	RotationToPlayer = InRotationToPlayer;
	Length = InLength;
	Height = InHeight;
	Width = InWidth;
	Radius = InRadius;
	MaxVerticalRange = InMaxVerticalRange;
	MaxHorizontalRange = InMaxHorizontalRange;
	ShouldProduceTargetDataOnServer = bInShouldProduceTargetDataOnServer;
	bRequiresAiming = bInRequiresAiming;
	bTraceOriginShouldBeCameraXY = bInTraceOriginShouldBeCameraXY;
	bDebug = bInDebug;
}


void AP_GATA_GroundDeploy::DetermineSpawnParameters()
{
	switch (ActorShape)
	{
		case EDeployTargetActorShape::Capsule:
			DetermineCapsuleParameters();
			break;
		case EDeployTargetActorShape::Box:
			DetermineBoxParameters();
			break;
		case EDeployTargetActorShape::Sphere:
			DetermineSphereParameters();
			break;
		default: ;
	}
}

void AP_GATA_GroundDeploy::DetermineCapsuleParameters()
{
	// GroundOffset = Radius;
	WallDistance = Radius;
}

void AP_GATA_GroundDeploy::DetermineBoxParameters()
{
	// GroundOffset = Height * 0.5f;
	WallDistance = Length * 0.5f;
}

void AP_GATA_GroundDeploy::DetermineSphereParameters()
{
	// GroundOffset = Radius * 0.5f;
	WallDistance = Radius;
}


void AP_GATA_GroundDeploy::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	bFirstTick = true;
	SetActorTickEnabled(true);
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();

	DetermineSpawnParameters();
	// const FVector Scale = {Length, Width, Height};
	// // Mesh->SetWorldScale3D(Scale);
	// TargetingLocationInfo.LiteralTransform.SetScale3D(Scale);
}

void AP_GATA_GroundDeploy::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData());

		const FGameplayAbilityTargetDataHandle Handle = MakeTargetData(TargetingLocationInfo);
		TargetDataReadyDelegate.Broadcast(Handle);
}

void AP_GATA_GroundDeploy::CancelTargeting()
{
	const FGameplayAbilityActorInfo* ActorInfo = (IsValid(OwningAbility) ? OwningAbility->GetCurrentActorInfo() : nullptr);
	UAbilitySystemComponent* AbilitySystem = (ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr);
	if (IsValid(AbilitySystem))
	{
		AbilitySystem->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::GenericCancel, OwningAbility->GetCurrentAbilitySpecHandle(), OwningAbility->GetCurrentActivationInfo().GetActivationPredictionKey()).Remove(GenericCancelHandle);
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("AGameplayAbilityTargetActor::CancelTargeting called with null ASC! Actor %s"), *GetName());
	}

	CanceledDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
}

void AP_GATA_GroundDeploy::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	
}

void AP_GATA_GroundDeploy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AP_GATA_GroundDeploy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds); //Maybe ChangeTickInterval
	
	TargetingLocationInfo = PerformGroundDeployTrace(SourceActor);
	if(!bWasLastTraceValid)
	{
		SetActorHiddenInGame(true);
	}
	else
	{
		if(PrimaryPC && PrimaryPC->IsLocalController())
		{
			SetActorHiddenInGame(false);
			UpdateTargetActorTransform(TargetingLocationInfo.LiteralTransform);
		}
	}
}

void AP_GATA_GroundDeploy::UpdateTargetActorTransform(const FTransform& Transform)
{
	SetActorTransform(Transform);
}

void AP_GATA_GroundDeploy::StopTargeting()
{
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);

	TargetDataReadyDelegate.Clear();
	CanceledDelegate.Clear();

	if (GenericDelegateBoundASC)
	{
		GenericDelegateBoundASC->GenericLocalConfirmCallbacks.RemoveDynamic(this, &AGameplayAbilityTargetActor::ConfirmTargeting);
		GenericDelegateBoundASC->GenericLocalCancelCallbacks.RemoveDynamic(this, &AGameplayAbilityTargetActor::CancelTargeting);
		GenericDelegateBoundASC = nullptr;
	}
}

FGameplayAbilityTargetingLocationInfo AP_GATA_GroundDeploy::PerformGroundDeployTrace(TObjectPtr<AActor> InSourceActor)
{
	FGameplayAbilityTargetingLocationInfo EndLocationInfo = FGameplayAbilityTargetingLocationInfo();
	EndLocationInfo.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	EndLocationInfo.SourceAbility = StartLocation.SourceAbility;
	EndLocationInfo.SourceActor = StartLocation.SourceActor;
	EndLocationInfo.SourceComponent = StartLocation.SourceComponent;
	EndLocationInfo.SourceSocketName = StartLocation.SourceSocketName;
	
	FTransform StartTransform = StartLocation.GetTargetingTransform();
	FTransform EndTransform = FTransform::Identity;
	const FVector Scale = {Length, Width, Height};
	// Mesh->SetWorldScale3D(Scale);
	EndTransform.SetScale3D(Scale);
	

	bool bTraceComplex = false;
	// TArray<AActor*> ActorsToIgnore;

	// ActorsToIgnore.Add(InSourceActor);
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(AGameplayAbilityTargetActor_GroundTrace), bTraceComplex);
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(InSourceActor);
	
	const TWeakObjectPtr<AProtelumCharacter> PlayerCharacter = Cast<AProtelumCharacter>(InSourceActor);
	if(!PlayerCharacter.IsValid())
	{
		UE_LOG(LogTemp, Warning , TEXT("%s %s() PlayerCharacter is not Valid"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__));
		bWasLastTraceValid = false;
		return FGameplayAbilityTargetingLocationInfo();
	}
	FVector PlayerLocation;
	FVector CameraLocation;
	FVector WeaponCenterLocation;
	FVector CameraForwardVector;
	PlayerCharacter->GetTargetingData(PlayerLocation, CameraLocation, WeaponCenterLocation, CameraForwardVector);
	float DistanceCameraToPlayer = 0.0f;
	if(bTraceOriginShouldBeCameraXY)
	{
		CameraLocation.Z = StartTransform.GetLocation().Z;
		StartTransform.SetLocation(CameraLocation);
		
		PlayerLocation.Z = 0.0f;
		CameraLocation.Z = 0.0f;
		DistanceCameraToPlayer = FVector::Distance(PlayerLocation, CameraLocation);
	}

	TWeakObjectPtr<UWorld> World = GetWorld();
	check(World.Get())
	
	FVector HorizontalTraceDirection = CameraForwardVector.GetSafeNormal2D();
	FVector HorizontalTraceEndLocation = StartTransform.GetLocation() + HorizontalTraceDirection * (MaxHorizontalRange + DistanceCameraToPlayer);
	FVector HorizontalTraceStartLocation = StartTransform.GetLocation();
	
	FTransform InTransform = PerformHorizontalTrace(HorizontalTraceStartLocation, HorizontalTraceDirection , HorizontalTraceEndLocation, QueryParams);
	EndTransform.SetRotation(InTransform.GetRotation());

	FVector VerticalTraceStartLocation = InTransform.GetLocation();
	FVector VerticalTraceEndLocation = VerticalTraceStartLocation;
	VerticalTraceEndLocation.Z =- MaxVerticalRange;

	FHitResult VerticalHitResult;
	PerformSimpleTrace(VerticalHitResult, VerticalTraceStartLocation, VerticalTraceEndLocation,
		TraceProfile.Name, QueryParams, World);
	if(VerticalHitResult.bBlockingHit)
	{
		FVector EndLocation = VerticalHitResult.ImpactPoint;
		EndLocation.Z =+ GroundOffset;
		EndTransform.SetLocation(EndLocation);
	}
	else //This is very Costy maybe Reduce the Steps or Clear this Section eventhough this is rare
	{
		FTransform TempTransform;
		FVector LoopBackHorizontalTraceDirection = HorizontalTraceDirection * -1;
		FVector LoopBackHorizontalStartLocation = HorizontalTraceEndLocation;
		FVector LoopBackHorizontalEndLocation = StartTransform.GetLocation();
		float DistanceBetweenStartAndEnd = FVector::Dist2D(LoopBackHorizontalStartLocation, LoopBackHorizontalEndLocation);
		int StepCount = 10;
		for(int Step = 1; Step <= StepCount ; Step++)
		{
			LoopBackHorizontalEndLocation = LoopBackHorizontalStartLocation + LoopBackHorizontalTraceDirection * (DistanceBetweenStartAndEnd / StepCount) * Step;
			TempTransform = PerformHorizontalTrace(LoopBackHorizontalStartLocation, LoopBackHorizontalTraceDirection, LoopBackHorizontalEndLocation, QueryParams);
			VerticalHitResult.Reset(-1);

			FVector LoopVerticalTraceStartLocation = TempTransform.GetLocation();
			FVector LoopVerticalTraceEndLocation = VerticalTraceStartLocation;
			VerticalTraceEndLocation.Z =- MaxVerticalRange;
			PerformSimpleTrace(VerticalHitResult, LoopVerticalTraceStartLocation, LoopVerticalTraceEndLocation,
				TraceProfile.Name, QueryParams, World);
			if(VerticalHitResult.bBlockingHit)
			{
				FVector EndLocation = VerticalHitResult.ImpactPoint;
				EndLocation.Z =+ GroundOffset;
				EndTransform.SetLocation(EndLocation);
				break;
			}
		}
		if(!VerticalHitResult.bBlockingHit)
		{
			bWasLastTraceValid = false;
			return StartLocation;
		}
	}
	bWasLastTraceValid = true;
	EndLocationInfo.LiteralTransform = EndTransform;
	return EndLocationInfo;
}

FTransform AP_GATA_GroundDeploy::PerformHorizontalTrace(
	FVector& TraceStartLocation, FVector& TraceDirection, FVector& TraceEndLocation, FCollisionQueryParams Params) const
{

	FTransform OutTransform = FTransform::Identity;
	
		TWeakObjectPtr<UWorld> World = GetWorld();
	check(World.Get())
	FHitResult HorizontalHitResult;
	PerformSimpleTrace(HorizontalHitResult, TraceStartLocation, TraceEndLocation,
		TraceProfile.Name, Params, World);
	if(!HorizontalHitResult.bBlockingHit)
	{
		FVector HorizontalCircleStartLocation = TraceEndLocation;
		FVector HorizontalCircleEndLocation = FVector::ZeroVector;
		FRotator HorizontalCircleRotation = FRotator::ZeroRotator;
		float ShortestDistance = WallDistance;
		for(float i = 0; i <= 360; i++) //Todo: Find a different Solution on the long run!!!!
		{
			FHitResult HorizontalCircleHitResult;
			HorizontalCircleRotation.Yaw = i;
			HorizontalCircleEndLocation = HorizontalCircleStartLocation + HorizontalCircleRotation.RotateVector(TraceDirection) * WallDistance;
			PerformSimpleTrace(HorizontalCircleHitResult, HorizontalCircleStartLocation, HorizontalCircleEndLocation, TraceProfile.Name, Params, World);
			if(HorizontalCircleHitResult.bBlockingHit && HorizontalCircleHitResult.Distance < ShortestDistance)
			{
				ShortestDistance = HorizontalCircleHitResult.Distance;
				HorizontalHitResult = HorizontalCircleHitResult;
			}
		}
	}
	FVector Desired2DLocation;
	FQuat DesiredRotation;
	if(HorizontalHitResult.bBlockingHit)
	{
		FVector OppositeDirection = HorizontalHitResult.ImpactNormal;
		OppositeDirection.Z = 0;
		FVector EndLocation2D = HorizontalHitResult.ImpactPoint + OppositeDirection * WallDistance;
		Desired2DLocation = EndLocation2D;

		FRotator Rotation = FRotator::ZeroRotator;
		Rotation.Yaw = 90.f;//Aligns the Shape to the Rotation of the Wall, so the Rotation to Player matters
		const FRotator HitRotation = OppositeDirection.ToOrientationRotator() - Rotation;
		const FRotator DeltaRotation = HitRotation - RotationToPlayer;
		DesiredRotation = DeltaRotation.Quaternion();
	}
	else
	{
		const FRotator OriginRotation = TraceDirection.ToOrientationRotator();
		const FRotator DeltaRotation = OriginRotation - RotationToPlayer;
		DesiredRotation = DeltaRotation.Quaternion();
		Desired2DLocation = HorizontalHitResult.TraceEnd;
	}
	OutTransform.SetRotation(DesiredRotation);
	OutTransform.SetLocation(Desired2DLocation);
	FVector Scale = {Length, Width, Height};
	OutTransform.SetScale3D(Scale);
	return OutTransform;
}

void AP_GATA_GroundDeploy::PerformSimpleTrace(FHitResult& OutHitResult, const FVector& StartTraceLocation, const FVector& EndTraceLocation, const FName& ProfileName,
	const FCollisionQueryParams& Params, const TWeakObjectPtr<UWorld> World) const
{
	World->LineTraceSingleByProfile(OutHitResult, StartTraceLocation, EndTraceLocation, ProfileName, Params);
	#if ENABLE_DRAW_DEBUG
			if (bDebug)
			{
				DrawDebugLine(World.Get(), StartTraceLocation, EndTraceLocation, FColor::Red);
			}
	#endif
}

bool AP_GATA_GroundDeploy::IsConfirmTargetingAllowed()
{
	return bWasLastTraceValid;
}

FGameplayAbilityTargetDataHandle AP_GATA_GroundDeploy::MakeTargetData(const FGameplayAbilityTargetingLocationInfo& LocationInfo)
{
	FGameplayAbilityTargetDataHandle ReturnDataHandle;
	
	/** Note: These are cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
	FGameplayAbilityTargetData_LocationInfo* ReturnData = new FGameplayAbilityTargetData_LocationInfo();
	//Need to Manipulate this HitResult or Modify the Data Afterwards in the Ability to Make the Final One Spawn exactly there
	ReturnData->TargetLocation = LocationInfo;
	
	ReturnDataHandle.Add(ReturnData);
	// FGameplayAbilityTargetData* TempTestData = ReturnDataHandle.Get(0);
	// FGameplayAbilityTargetData_LocationInfo* NewTestData = Cast<FGameplayAbilityTargetData_LocationInfo>(TempTestData);
	// NewTestData->TargetLocation;
	return ReturnDataHandle;
}
