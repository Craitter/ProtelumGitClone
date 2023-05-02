// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProtelumSpringArmComponent.generated.h"

/**
 * 
 */


UENUM()
enum class ECameraPositionState
{
	CPS_Set = 0		UMETA(DisplayName = "Set"),
	CPS_Switching = 1	UMETA(DisplayName = "Switching")
};

DECLARE_DELEGATE(FOnOffsetSet);

UCLASS()
class PROTELUMTOY_API UProtelumSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

	
	UProtelumSpringArmComponent();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SwitchSpringArmShoulder();
	
	FOnOffsetSet OffsetSet;
protected:
	virtual void BeginPlay() override;

private:
	ECameraPositionState CameraPositionState = ECameraPositionState::CPS_Set;

	float CurrentCameraSwitchingTime = 0.0f;

	UPROPERTY(EditAnywhere)
	float CameraSwitchingTime = 0.1f;

	FVector StartOffset = FVector::ZeroVector;
	FVector TargetOffset = FVector::ZeroVector;
};
