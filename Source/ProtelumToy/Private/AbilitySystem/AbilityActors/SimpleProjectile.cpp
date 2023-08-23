// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityActors/SimpleProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "NiagaraComponent.h"
#include "Engine/OverlapInfo.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASimpleProjectile::ASimpleProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	SetReplicatingMovement(true);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(Mesh);
	if(IsValid(Mesh))
	{
		//Todo: Make it Affect by Hit and not by Overlap
		
		// Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Mesh->SetGenerateOverlapEvents(true);
		Mesh->SetCollisionProfileName("Projectile");
		Mesh->SetIsReplicated(true);
		// FCollisionResponseContainer(ECR_Ignore, ECR_Ignore,)
	}

	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	if(IsValid(ProjectileMovementComponent))
	{
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	if(IsValid(NiagaraSystem))
	{
		NiagaraSystem->SetupAttachment(Mesh);
	}
}


void ASimpleProjectile::BeginPlay()
{
	Super::BeginPlay();

	const TWeakObjectPtr<AActor> InstigatorActor = GetInstigator();
	if(InstigatorActor.IsValid())
	{
		const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(InstigatorActor);
		if(ASI != nullptr)
		{
			SourceASC = ASI->GetAbilitySystemComponent();
		}
	}
}

void ASimpleProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ASimpleProjectile, ProjectileColor, COND_None, REPNOTIFY_Always);
}

void ASimpleProjectile::OnRep_ProjectileColor()
{
	SetProjectileColor();
}

void ASimpleProjectile::PostNetReceiveVelocity(const FVector& NewVelocity)
{
	Super::PostNetReceiveVelocity(NewVelocity);

	if(IsValid(ProjectileMovementComponent))
	{
		ProjectileMovementComponent->Velocity = NewVelocity;
		ProjectileMovementComponent->UpdateComponentVelocity();
	}
}

void ASimpleProjectile::ConfigureProjectile(
	const EProjectileType InProjectileType,
	const TArray<FGameplayEffectSpecHandle>& InInstigatorEffects, const TArray<FGameplayEffectSpecHandle>&
	InTargetEffects,
	const float InAffectionRadius, const float InProjectileLifeSpan, const FLinearColor& InProjectileColor, const
	FVector& InVelocityInLocalSpace, const bool InDebug)
{
	ProjectileType = InProjectileType;
	EffectsToApplyToInstigator = InInstigatorEffects;
	EffectsToApplyToTarget = InTargetEffects;
	AffectionRadius = InAffectionRadius;
	ProjectileLifeSpan = InProjectileLifeSpan;
	ProjectileColor = InProjectileColor;
	VelocityInLocalSpace = InVelocityInLocalSpace;

	//Todo: MaybeCollisionProfile
	//Todo: Particles for ProjectileHit

#if ENABLE_DRAW_DEBUG
	bDebug = InDebug;
#endif
	SetProjectileColor();
	SetProjectileVelocity();
	SetLifeSpan(ProjectileLifeSpan);
	ActivateCollisions();
}

void ASimpleProjectile::ConfirmByInstigator()
{
	OnProjectileLifeEnds.Broadcast(GetActorLocation(), false);
}

void ASimpleProjectile::ActivateCollisions()
{
	OnProjectileLifeEnds.AddDynamic(this, &ASimpleProjectile::CallbackLifeSpawnEnds);
	
	if(IsValid(Mesh))
	{
		Mesh->OnComponentBeginOverlap.AddDynamic(this, &ASimpleProjectile::OnOverlapBegin);
		TArray<FOverlapInfo> InOverlapInfo = Mesh->GetOverlapInfos();
		for(FOverlapInfo Info : InOverlapInfo)
		{
			AActor* OverlapActor = Info.OverlapInfo.GetActor();
			if(OverlapActor != GetInstigator() && OverlapActor != nullptr)
			{
				UPrimitiveComponent* OtherComponent = Info.OverlapInfo.GetComponent();
				Mesh->OnComponentBeginOverlap.Broadcast(Mesh, OverlapActor, OtherComponent, 0, true, Info.OverlapInfo);
			}
		}
		Mesh->OnComponentHit.AddDynamic(this, &ASimpleProjectile::OnHitCallback);
	}
}

void ASimpleProjectile::HandleCollision(const TWeakObjectPtr<AActor> CollisionActor, const FHitResult& HitResult)
{
	TWeakObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;
	const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(CollisionActor);
	if(ASI != nullptr)
	{
		TargetASC = ASI->GetAbilitySystemComponent();
	}

	const FVector ProjectileLocation = GetActorLocation();
	switch (ProjectileType)
	{
	case EProjectileType::Explosive:
		OnProjectileLifeEnds.Broadcast(ProjectileLocation, true);
		Explode();
		break;
	case EProjectileType::SingleTarget:
		OnProjectileLifeEnds.Broadcast(ProjectileLocation, true);
		SingleTargetHit(TargetASC);
		break;
	case EProjectileType::PenetrateNonMassive:
		SingleTargetHit(TargetASC);
		if(!HitResult.bBlockingHit)
		{
			return;
		}
		OnProjectileLifeEnds.Broadcast(ProjectileLocation, true);
		break;
	default: ;
	}
	
	DestroyProjectile(false);
}

void ASimpleProjectile::SetProjectileColor()
{
	if(!IsValid(Mesh))
	{
		return;
	}
	if(!IsValid(DynamicMaterial))
	{
		DynamicMaterial = Mesh->CreateDynamicMaterialInstance(0);
	}
	DynamicMaterial->SetVectorParameterValue(FName("Emissive_Color"), ProjectileColor);
	Mesh->SetMaterial(0, DynamicMaterial);

	// if(IsValid(NiagaraSystem))
	// {
	// 	NiagaraSystem->SetColorParameter("Color", ProjectileColor);
	// }
}

void ASimpleProjectile::SetProjectileVelocity() const
{
	if(IsValid(ProjectileMovementComponent))
	{
		ProjectileMovementComponent->SetVelocityInLocalSpace(VelocityInLocalSpace);
		ProjectileMovementComponent->UpdateComponentVelocity();
	}
}


UProjectileMovementComponent* ASimpleProjectile::GetProjectileMovement()
{
	return ProjectileMovementComponent;
}

USkeletalMeshComponent* ASimpleProjectile::GetSkeletalMeshComponent()
{
	return Mesh;
}

void ASimpleProjectile::OnHitCallback(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor == GetInstigator())
	{
		DestroyProjectile(false);
		return;
	}
	if(GetLocalRole() != ROLE_Authority)
	{
		DestroyProjectile(false);;
		return;
	}
	HandleCollision(OtherActor, Hit);
}

void ASimpleProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == GetInstigator())
	{
		return;
	}
	if(GetLocalRole() != ROLE_Authority)
	{
		DestroyProjectile(false);;
		return;
	}
	HandleCollision(OtherActor, SweepResult);
}

void ASimpleProjectile::SetLifeSpan(float InLifespan)
{
	if(InLifespan <= 0.0f)
	{
		InLifespan = 30.0f;
	}
	Super::SetLifeSpan(InLifespan);
}

void ASimpleProjectile::LifeSpanExpired()
{
	OnProjectileLifeEnds.Broadcast(GetActorLocation(), false);
}

void ASimpleProjectile::CallbackLifeSpawnEnds(const FVector& ActorLocation, bool bAlreadyTriggered)
{
	OnProjectileLifeEnds.Clear();
	if(!bAlreadyTriggered)
	{
		HandleCollision(nullptr, FHitResult());
	}
	
}

void ASimpleProjectile::Explode() const
{
	const TWeakObjectPtr<UWorld> World = GetWorld();
	TArray<FHitResult> SweepHitResults;
	const FVector StartLocation = GetActorLocation();
	FCollisionShape SphereShape;
	SphereShape.SetSphere(AffectionRadius);
	
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(AGameplayAbilityTargetActor_GroundTrace), false);
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(this);
	FName CollisionProfile;
	if(IsValid(Mesh))
	{
		CollisionProfile = Mesh->GetCollisionProfileName();
	}
	World->SweepMultiByProfile(SweepHitResults, StartLocation, StartLocation, FQuat::Identity, CollisionProfile, SphereShape, QueryParams);
#if ENABLE_DRAW_DEBUG
	if (bDebug)
	{
		DrawDebugSphere(World.Get(), StartLocation, AffectionRadius, 20.0f, FColor::Red, false, 3);
	}
#endif
	for(const FHitResult& Hit : SweepHitResults)
	{
		TWeakObjectPtr<AActor> HitActor = Hit.GetActor();
		if(const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(HitActor))
		{
			TWeakObjectPtr<UAbilitySystemComponent> ASC = ASI->GetAbilitySystemComponent();
			if(ASC.IsValid())
			{
				FHitResult DoesItHitResult;
				World->LineTraceSingleByProfile(DoesItHitResult, StartLocation, HitActor->GetActorLocation(), "Projectile");
#if ENABLE_DRAW_DEBUG
				if (bDebug)
				{
					DrawDebugLine(World.Get(), StartLocation, HitActor->GetActorLocation(), FColor::Blue, false, 3);
				}
#endif
				if(DoesItHitResult.bBlockingHit && HitActor == DoesItHitResult.GetActor())
				{
					SingleTargetHit(ASC);
				}
			}
			//Todo: Change this so particles can effect every hit surface if needed
		}
	}
}

void ASimpleProjectile::SingleTargetHit(TWeakObjectPtr<UAbilitySystemComponent> TargetASC) const
{
	if(TargetASC.IsValid())
	{
		ApplyEffectsAfterHit(TargetASC);
	}
}

void ASimpleProjectile::ApplyEffectsAfterHit(const TWeakObjectPtr<UAbilitySystemComponent> TargetASC) const
{
	ApplyEffectsToASC(TargetASC, EffectsToApplyToTarget);

	ApplyEffectsToASC(SourceASC, EffectsToApplyToInstigator);
}

void ASimpleProjectile::ApplyEffectsToASC(const TWeakObjectPtr<UAbilitySystemComponent> ASC,
                                          const TArray<FGameplayEffectSpecHandle>& GameplayEffectsToApply)
{
	if(ASC.IsValid())
	{
		for(const FGameplayEffectSpecHandle& GEToApply : GameplayEffectsToApply)
		{
			if(GEToApply.IsValid())
			{
				
				// FActiveGameplayEffectHandle ActiveGEHandle =
				ASC->ApplyGameplayEffectSpecToTarget(*GEToApply.Data.Get(), ASC.Get());
			}
		}
	}
}

void ASimpleProjectile::DestroyProjectile(bool NetForce)
{
	//Todo: Things to do before destroying
	Destroy(NetForce);
}

void ASimpleProjectile::PreDestroyFromReplication()
{
	DestroyProjectile(true);
	Super::PreDestroyFromReplication();
}
