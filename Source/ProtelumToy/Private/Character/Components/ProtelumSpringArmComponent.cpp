// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/ProtelumSpringArmComponent.h"


UProtelumSpringArmComponent::UProtelumSpringArmComponent()
{
	bUsePawnControlRotation = true;
	TargetArmLength = 400.0f;
	SocketOffset = {0.0f, 80.0f, 0.0f};
}

void UProtelumSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CameraPositionState == ECameraPositionState::CPS_Switching)
	{
		CurrentCameraSwitchingTime += DeltaTime;
		const float TimeRatio = FMath::Clamp(CurrentCameraSwitchingTime / CameraSwitchingTime, 0.0f, 1.0f);
		// const float RotationAlpha = OpenCurve.GetRichCurveConst()->Eval(TimeRatio); IF WE WANT A CUSTOM SWITCHING CURVE
		const FVector CurrentTargetOffset = FMath::Lerp(StartOffset, TargetOffset, TimeRatio);//Todo: Maybe Interpolate to make it feel more dynamic
		SocketOffset = CurrentTargetOffset;
		if(TimeRatio >= 1.0f)
		{
			CameraPositionState = ECameraPositionState::CPS_Set;
			CurrentCameraSwitchingTime = 0.0f;
			OffsetSet.Execute();
		}
	}
}

void UProtelumSpringArmComponent::SwitchSpringArmShoulder()
{
	if(CameraPositionState != ECameraPositionState::CPS_Set)
	{
		CurrentCameraSwitchingTime = CameraSwitchingTime * 0.95;
	}
	else
	{
		StartOffset = SocketOffset;
		TargetOffset.Y = StartOffset.Y * -1;
		CameraPositionState = ECameraPositionState::CPS_Switching;
	}
}

void UProtelumSpringArmComponent::BeginPlay()
{
	Super::BeginPlay();
	
	StartOffset = SocketOffset;
	TargetOffset = SocketOffset;
}

FVector UProtelumSpringArmComponent::BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation,
	bool bHitSomething, float DeltaTime)
{
	// Super::BlendLocations(DesiredArmLocation, TraceHitLocation, bHitSomething, DeltaTime);
	if(!bHitSomething)
	{
		return Super::BlendLocations(DesiredArmLocation, TraceHitLocation, bHitSomething, DeltaTime);
	}
	const FVector SpringArmSocketLocation = GetRelativeLocation() + GetOwner()->GetActorLocation();
	const FVector CameraSocketLocation = GetSocketLocation(SocketName);
	const FVector CameraToPlayerVector = (SpringArmSocketLocation - CameraSocketLocation).GetSafeNormal();
	const FVector CameraToDesiredVector = (DesiredArmLocation - CameraSocketLocation).GetSafeNormal();
	const float DotResult = FVector::DotProduct(CameraToPlayerVector, CameraToDesiredVector);
	if(DotResult <= 0.0f)
	{
		// UE_LOG(LogTemp, Warning , TEXT("%s() CameraBlockWasBehindCamera"), *FString(__FUNCTION__));
	}
	else
	{
		//Todo: Find a way to handle both cases
		// UE_LOG(LogTemp, Warning , TEXT("%s() CameraBlockWasBeforeCamera"), *FString(__FUNCTION__));
	}
	return  TraceHitLocation;
}
