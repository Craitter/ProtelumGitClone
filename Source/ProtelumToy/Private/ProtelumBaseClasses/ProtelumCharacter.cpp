// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtelumBaseClasses/ProtelumCharacter.h"


#include "NiagaraComponent.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "AbilitySystem/ProtelumAbilitySystemComponent.h"
#include "AbilitySystem/AbilityActors/SimpleProjectile.h"
#include "AbilitySystem/AttributeSets/StaticActorAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Character/Components/ProtelumSpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AProtelumCharacter::AProtelumCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Stops Pawn from Rotation with Controller
	this->bUseControllerRotationYaw = false;
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationRoll = false;
	bReplicates = true;
	
	const TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent = GetCharacterMovement();
	if(IsValid(CharacterMovementComponent))
	{		
		//The Rate the Pawn Rotates to Fit the Controller Rotation
		CharacterMovementComponent->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
		//Enabling Crouch
		CharacterMovementComponent->NavAgentProps.bCanCrouch = true;

		CharacterMovementComponent->JumpZVelocity = 500.0f;
		
		CharacterMovementComponent->bUseControllerDesiredRotation = false;
		CharacterMovementComponent->bOrientRotationToMovement = true;

		// UE_LOG(LogTemp, Warning , TEXT("%s %s() %f, %f "), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__),
			// CharacterMovementComponent->GetPredictionData_Client_Character()->MaxSmoothNetUpdateDist, CharacterMovementComponent->GetPredictionData_Client_Character()->NoSmoothNetUpdateDist  );
		CharacterMovementComponent->GetPredictionData_Client_Character()->MaxSmoothNetUpdateDist = 92.0f;
		CharacterMovementComponent->GetPredictionData_Client_Character()->NoSmoothNetUpdateDist = 140.0f;
	}
	
	SpringArm = CreateDefaultSubobject<UProtelumSpringArmComponent>(TEXT("SpringArm"));
	if(ensure(IsValid(SpringArm)))
	{
		SpringArm->SetupAttachment(RootComponent);
	}
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if(ensure(IsValid(Camera)))
	{
		Camera->SetupAttachment(SpringArm);
	}
	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSKMesh");
	if(ensure(IsValid(WeaponComponent)))
	{
		WeaponComponent->SetupAttachment(GetMesh(), FName("weapon_r"));
	}
	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>("NiagaraSystem");
	if(ensure(IsValid(NiagaraSystem)))
	{
		NiagaraSystem->SetupAttachment(RootComponent);
	}
	WeaponNiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>("WeaponNiagaraSystem");
	if(ensure(IsValid(WeaponNiagaraSystem)))
	{
		WeaponNiagaraSystem->SetupAttachment(WeaponComponent);
	}
}

// Called when the game starts or when spawned
void AProtelumCharacter::BeginPlay()
{
	Super::BeginPlay();
	MState = EMovementState::MS_Walk;

	CurrentWeaponTag = PrimaryWeaponTag;
}

// Called every frame
void AProtelumCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Todo:This Should be driven by Events! (Maybe in the MovementComponent?)
	if(bMStateChanged)
	{
		const TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent = GetCharacterMovement();
		if(IsValid(CharacterMovementComponent))
		{
			if(MState == EMovementState::MS_Walk)
			{
				CharacterMovementComponent->bOrientRotationToMovement = true;
				CharacterMovementComponent->bUseControllerDesiredRotation = false;				
			}
			else if(MState == EMovementState::MS_Aim)
			{
				CharacterMovementComponent->bOrientRotationToMovement = false;
				CharacterMovementComponent->bUseControllerDesiredRotation = true;
				// FRotator OutRotation = GetActorRotation();
				// const FRotator TempRotation = Camera->GetComponentRotation();
				// OutRotation.Yaw = TempRotation.Yaw;
				// SetActorRotation(OutRotation);
			}
		}
		bMStateChanged = false;
	}
	DrawDebugString(GetWorld(),FVector(0, 0, 100),UEnum::GetValueAsString(GetLocalRole()), this, FColor::Green,DeltaTime);
}


void AProtelumCharacter::SetMState(EMovementState NewMState)
{
	MState = NewMState;
	bMStateChanged = true;
}

AGameplayAbilityTargetActor* AProtelumCharacter::GetTargetActorReference(UGameplayAbility* ReferencedAbility, TSubclassOf<AGameplayAbilityTargetActor> TargetActorClass)
{
	if(!GameplayAbilityTargetActor)
	{
		GameplayAbilityTargetActor = Cast<AGameplayAbilityTargetActor>(GetWorld()->SpawnActor(TargetActorClass));
		GameplayAbilityTargetActor->SetOwner(this);
		GameplayAbilityTargetActor->SetInstigator(this);
	}
	return GameplayAbilityTargetActor;
	// for(const auto TargetActorMember : TargetActorInfos)
	// {
	// 	if(TargetActorMember.AbilityReference->StaticClass() == ReferencedAbility->StaticClass())
	// 	{
	// 		return TargetActorMember.TargetActor;
	// 	} //Todo: Make this work properly maybe the problem is inside of the TargetActor, next check will be if a single pointer is enough
	// }	
	// return nullptr;
}

void AProtelumCharacter::SetTargetActorReference(UGameplayAbility* ReferencedAbility,
	AGameplayAbilityTargetActor* ReferencedTargetActor)
{
	// FTargetActorInfo Info;
	// Info.AbilityReference = ReferencedAbility;
	// Info.TargetActor = ReferencedTargetActor;
	// TargetActorInfos.Add(Info);
}

void AProtelumCharacter::PrimaryWeapon()
{
	SetCurrentWeapon(PrimaryWeaponTag);
}

void AProtelumCharacter::SecondaryWeapon()
{
	SetCurrentWeapon(SecondaryWeaponTag);
}

void AProtelumCharacter::NextWeapon()
{
	FGameplayTag WeaponTag;
	if(CurrentWeaponTag == PrimaryWeaponTag)
	{
		WeaponTag = SecondaryWeaponTag;
	}
	else if(CurrentWeaponTag == SecondaryWeaponTag)
	{
		WeaponTag = PrimaryWeaponTag;
	}
	SetCurrentWeapon(WeaponTag);
}

void AProtelumCharacter::PreviousWeapon()
{
	FGameplayTag WeaponTag;
	if(CurrentWeaponTag == PrimaryWeaponTag)
	{
		WeaponTag = SecondaryWeaponTag;
	}
	else if(CurrentWeaponTag == SecondaryWeaponTag)
	{
		WeaponTag = PrimaryWeaponTag;
	}
	SetCurrentWeapon(WeaponTag);
}

void AProtelumCharacter::ActivateCurrentWeapon() const
{
	if(ProtelumAbilitySystem.IsValid() && GetLocalRole() == ROLE_Authority)
	{
		ProtelumAbilitySystem->TryActivateAbilitiesByTag(FGameplayTagContainer(CurrentWeaponTag));
	}
}


void AProtelumCharacter::SetCurrentWeapon(FGameplayTag WeaponTag)
{
	CurrentWeaponTag = WeaponTag; //Here we can do a lot more or we could make a GA_Switch to make animations and co...
}




