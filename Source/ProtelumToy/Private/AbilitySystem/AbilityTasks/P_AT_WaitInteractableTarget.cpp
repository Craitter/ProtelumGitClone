// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/P_AT_WaitInteractableTarget.h"

#include "AbilitySystem/Interface/ProtelumInteractable.h"
#include "ProtelumBaseClasses/ProtelumCharacter.h"
#include "ProtelumToy/ProtelumToy.h"

UP_AT_WaitInteractableTarget* UP_AT_WaitInteractableTarget::WaitForInteractableTarget(UGameplayAbility* OwningAbility,
                                                                                      FName TaskInstanceName, FGameplayAbilityTargetingLocationInfo StartLocation, FCollisionProfileName TraceProfile,
                                                                                      float MaxRange, float TimerPeriod, bool bShowDebug)
{
	UP_AT_WaitInteractableTarget* MyObj = NewAbilityTask<UP_AT_WaitInteractableTarget>(OwningAbility, TaskInstanceName);
	if(MyObj != nullptr)
	{
		MyObj->TraceProfile = TraceProfile;
		MyObj->MaxRange = MaxRange;
		MyObj->TimerPeriod = TimerPeriod;
		MyObj->bShowDebug = bShowDebug;
	}


	return MyObj;
}

void UP_AT_WaitInteractableTarget::Activate()
{
	const TWeakObjectPtr<UWorld> World = GetWorld();
	if(World.IsValid())
	{
		World->GetTimerManager().SetTimer(TraceTimerHandle, this, &UP_AT_WaitInteractableTarget::PerformTrace, TimerPeriod, true);
	}
}
 static void DoIt(int32 nice)
{
	int32 i = 7;
}
void UP_AT_WaitInteractableTarget::OnDestroy(bool AbilityEnded)
{
	const TWeakObjectPtr<UWorld> World = GetWorld();
	if(World.IsValid())
	{
		World->GetTimerManager().ClearTimer(TraceTimerHandle);
	}
	Super::OnDestroy(AbilityEnded);


	// // TODO: DOAsync
	// TUniquePtr<UP_AT_WaitInteractableTarget> Thing = MakeUnique<UP_AT_WaitInteractableTarget>();
	// //
	// // // * TFunction      <void()> CopyableFunc = [Thing = MoveTemp(Thing)](){ Thing->DoSomething(); }; // error - lambda is not copyable
	// // TUniqueFunction<void()> MovableFunc  = [Thing = MoveTemp(Thing)](){ Thing->PerformTrace(); }; // ok
	// TUniqueFunction<void()> MovableFunc  = [] { DoIt(6); }; // ok
	// AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, MoveTemp(MovableFunc));
	// //
}

void UP_AT_WaitInteractableTarget::LineTrace(FHitResult& OutHitResult, const UWorld* World, const FVector& Start,
                                             const FVector& End, FName ProfileName, const FCollisionQueryParams Params) const
{
	check(World);


	
	TArray<FHitResult> HitResults;
	World->LineTraceMultiByProfile(HitResults, Start, End, ProfileName, Params);
#if ENABLE_DRAW_DEBUG
	if(bShowDebug)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, TimerPeriod);
	}
#endif // ENABLE_DRAW_DEBUG
	OutHitResult.TraceStart = Start;
	OutHitResult.TraceEnd = End;
	OutHitResult.bBlockingHit = false;

	FHitResult InteractableHitResult;
	if(GetInteractable(HitResults, InteractableHitResult))
	{
		
		OutHitResult = InteractableHitResult;
		return;
	}

	FVector NewEndIfNothingWasInteractable = End;

	if(InteractableHitResult.bBlockingHit)
	{
		NewEndIfNothingWasInteractable = InteractableHitResult.Location;
	}
	
	FCollisionShape Sphere;
	Sphere.SetSphere(MaxRange / 5);
	
	HitResults.Empty();
	World->SweepMultiByProfile(HitResults, NewEndIfNothingWasInteractable, NewEndIfNothingWasInteractable, FQuat::Identity, ProfileName, Sphere, Params);
	
	if(GetInteractable(HitResults, InteractableHitResult))
	{
		OutHitResult = InteractableHitResult;
	}
#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		
		if (OutHitResult.bBlockingHit)
		{
			DrawDebugSphere(GetWorld(), OutHitResult.Location, MaxRange / 5, 16, FColor::Red, false, TimerPeriod);
		}
		else
		{
			DrawDebugSphere(GetWorld(), OutHitResult.TraceEnd, MaxRange / 5, 16, FColor::Green, false, TimerPeriod);
		}
	}
#endif // ENABLE_DRAW_DEBUG
}

bool UP_AT_WaitInteractableTarget::GetInteractable(const TArray<FHitResult>& InHitResults,
                                                   FHitResult& OutHitResult) const
{
	for (const FHitResult& Hit : InHitResults)
	{
		if(Hit.GetActor() && Hit.GetActor() != Ability->GetCurrentActorInfo()->AvatarActor.Get())
		{
			if(Hit.Component.IsValid() && Hit.Component.Get()->GetCollisionResponseToChannel(COLLISION_INTERACTABLE) == ECR_Overlap)
			{
				const bool bIsInteractable = Hit.GetActor()->Implements<UProtelumInteractable>();

				if(bIsInteractable && IProtelumInteractable::Execute_IsAvailableForInteraction(Hit.GetActor(), Hit.Component.Get()))
				{
					OutHitResult = Hit;
					OutHitResult.bBlockingHit = true;
					return true;
				}
			}
		}
		if(Hit.bBlockingHit)
		{
			OutHitResult = Hit; //Todo: Make this work cause there will be no blocking hit
		}
	}
	return false;
}

void UP_AT_WaitInteractableTarget::GetInteractionTraceStartAndEndLocation(FVector& Start, FVector& End, const UWorld* World, FName ProfileName, const FCollisionQueryParams& Params) const
{

	const TWeakObjectPtr<AProtelumCharacter> Character = Cast<AProtelumCharacter>(Ability->GetCurrentActorInfo()->AvatarActor.Get());
	if(!Character.IsValid())
	{
		UE_LOG(LogTemp, Warning , TEXT("%s() PLAYER CHARACTER IS EMPTY"), *FString(__FUNCTION__));
		return;
	}
	FVector PlayerLocation;
	FVector CameraLocation;
	FVector WeaponCenterLocation;
	FVector CameraForwardVector;
	Character->GetTargetingData(PlayerLocation, CameraLocation, WeaponCenterLocation, CameraForwardVector);

	const float DistanceCameraToPlayer = FVector::Distance(PlayerLocation, CameraLocation);
	
	Start = Character->GetPawnViewLocation();
	
	End = Start + CameraForwardVector.GetSafeNormal() * (MaxRange);// + DistanceCameraToPlayer);

	
	TArray<FHitResult> HitResults;
	World->LineTraceMultiByProfile(HitResults, Start, End, ProfileName, Params);
	for(const FHitResult& Hit : HitResults)
	{
		if(Hit.GetActor() && Hit.GetActor() != Ability->GetCurrentActorInfo()->AvatarActor.Get())
		{
			End = Hit.Location;
			return;
		}
	}
}

void UP_AT_WaitInteractableTarget::PerformTrace()
{
	constexpr bool bTraceComplex = false;
	TArray<AActor*> ActorsToIgnore;

	AActor* SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
	if (!SourceActor)
	{
		// Hero is dead
		//UE_LOG(LogTemp, Error, TEXT("%s %s SourceActor was null"), *FString(__FUNCTION__), *UGSBlueprintFunctionLibrary::GetPlayerEditorWindowRole(GetWorld()));
		return;
	}
	ActorsToIgnore.Add(SourceActor);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AGameplayAbilityTargetActor_SingleLineTrace), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);

	FVector TraceStart = FVector::ZeroVector;
	FVector TraceEnd = FVector::ZeroVector;
	GetInteractionTraceStartAndEndLocation(TraceStart, TraceEnd, GetWorld(), TraceProfile.Name, Params);
	if(TraceEnd == FVector::ZeroVector)
	{
		return;
	}
	FHitResult ReturnHitResult;
	LineTrace(ReturnHitResult, GetWorld(), TraceStart, TraceEnd, TraceProfile.Name, Params);

	// Default to end of trace line if we don't hit a valid, available Interactable Actor
	// bBlockingHit = valid, available Interactable Actor
	if (!ReturnHitResult.bBlockingHit)
	{
		// No valid, available Interactable Actor
		if (TargetData.Num() > 0 && TargetData.Get(0)->GetHitResult()->bBlockingHit)
		{
			// Previous trace had a valid Interactable Actor, now we don't have one
			// Broadcast last valid target
			LostInteractableTarget.Broadcast(TargetData);
		}

		TargetData = MakeTargetData(ReturnHitResult); //Why?
	}
	else
	{
		// Valid, available Interactable Actor

		bool bBroadcastNewTarget = true;

		if (TargetData.Num() > 0)
		{
			 AActor* OldTarget  = TargetData.Get(0)->GetHitResult()->GetActor();

			if (OldTarget == ReturnHitResult.GetActor())
			{
				// Old target is the same as the new target, don't broadcast the target
				bBroadcastNewTarget = false;
			}
			else if (OldTarget)
			{
				// Old target exists and is different from the new target
				// Broadcast last valid target
				LostInteractableTarget.Broadcast(TargetData);
			}
		}

		if (bBroadcastNewTarget)
		{
			// Broadcast new valid target
			TargetData = MakeTargetData(ReturnHitResult);
			FoundNewInteractableTarget.Broadcast(TargetData);
		}
	}
}

FGameplayAbilityTargetDataHandle UP_AT_WaitInteractableTarget::MakeTargetData(const FHitResult& HitResult) const
{
	/** Note: This will be cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
	return StartLocation.MakeTargetDataHandleFromHitResult(Ability, HitResult);

	
}
