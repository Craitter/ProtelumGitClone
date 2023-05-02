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
