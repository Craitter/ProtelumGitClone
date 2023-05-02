// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityActors/AreaOfEffectActor.h"

#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/ProtelumAbilitySystemComponent.h"
#include "Components/DecalComponent.h"

// Sets default values
AAreaOfEffectActor::AAreaOfEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	bReplicates = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if(IsValid(Mesh))
	{
		Mesh->SetUsingAbsoluteRotation(true);
		SetRootComponent(Mesh);
		Mesh->SetGenerateOverlapEvents(true);
		Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAreaOfEffectActor::OnOverlapBegin);
		Mesh->OnComponentEndOverlap.AddDynamic(this, &AAreaOfEffectActor::OnOverlapEnd);
	}
	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	if(IsValid(Decal))
	{
		Decal->SetupAttachment(Mesh);
		const FRotator Rotation = {90.0f, 0.0f, 0.0f};
		Decal->SetRelativeRotation(Rotation, false);
	}

	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	if(IsValid(NiagaraSystem))
	{
		NiagaraSystem->SetupAttachment(Mesh);
	}
}

void AAreaOfEffectActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const IAbilitySystemInterface* IAB = Cast<IAbilitySystemInterface>(OtherActor);
	if(!IAB)
	{
		return;
	}
	const TWeakObjectPtr<UProtelumAbilitySystemComponent> TargetASC = Cast<UProtelumAbilitySystemComponent>(IAB->GetAbilitySystemComponent());
	if(!TargetASC.IsValid())
	{
		return;
	}
	TWeakObjectPtr<UProtelumAbilitySystemComponent> ASC = TargetASC;
	if(InstigatorASC.IsValid())
	{
		ASC = InstigatorASC;
	}
	TArray<FActiveGameplayEffectHandle>* ActiveGameplayEffectHandles = &AffectedASCs.Add(TargetASC);;
	for(const FGameplayEffectSpecHandle& GEToApply : GameplayEffectsToApply)
	{
		if(GEToApply.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ASC->ApplyGameplayEffectSpecToTarget(*GEToApply.Data.Get(), TargetASC.Get());
			ActiveGameplayEffectHandles->Add(ActiveGameplayEffectHandle);
		}
	}
}

void AAreaOfEffectActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const IAbilitySystemInterface* IAB = Cast<IAbilitySystemInterface>(OtherActor);
	if(!IAB)
	{
		return;
	}
	const TWeakObjectPtr<UProtelumAbilitySystemComponent> TargetASC = Cast<UProtelumAbilitySystemComponent>(IAB->GetAbilitySystemComponent());
	if(!TargetASC.IsValid())
	{
		return;
	}

	if(TArray<FActiveGameplayEffectHandle>* ActiveGameplayEffectHandles = AffectedASCs.Find(TargetASC))
	{
		for(const FActiveGameplayEffectHandle ActiveGE : *ActiveGameplayEffectHandles)
		{
			TargetASC->RemoveActiveGameplayEffect(ActiveGE);
		}
	}

	AffectedASCs.Remove(TargetASC);
}

// Called when the game starts or when spawned
void AAreaOfEffectActor::BeginPlay()
{
	Super::BeginPlay();


	const FVector ActorScale = GetActorScale();
	const FVector CleanedScale = {ActorScale.Z, ActorScale.Y, ActorScale.X};
	Decal->SetWorldScale3D(CleanedScale);
	
	IAbilitySystemInterface* IAB = Cast<IAbilitySystemInterface>(GetInstigator());
	if(IAB)
	{
		InstigatorASC = Cast<UProtelumAbilitySystemComponent>(IAB->GetAbilitySystemComponent());
	}
	if(bShouldAttachToParent)
	{
		const FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
		AttachToActor(GetInstigator(), TransformRules, ParentSocket);
	}
}

void AAreaOfEffectActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for(const auto& AffectionEntry : AffectedASCs)
	{
		const TWeakObjectPtr<UProtelumAbilitySystemComponent> TargetASC = AffectionEntry.Key;
		TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles = AffectionEntry.Value;
		if(TargetASC.IsValid())
		{
			for(const FActiveGameplayEffectHandle ActiveGE : ActiveGameplayEffectHandles)
			{
				
				TargetASC->RemoveActiveGameplayEffect(ActiveGE);
			}
		}
	}
	AffectedASCs.Empty();
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AAreaOfEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

