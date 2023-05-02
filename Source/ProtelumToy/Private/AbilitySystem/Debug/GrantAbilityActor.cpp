// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DEbug/GrantAbilityActor.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystem/ProtelumAbilitySystemComponent.h"

// Sets default values
AGrantAbilityActor::AGrantAbilityActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	VisibleActor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(VisibleActor);
	VisibleActor->OnComponentBeginOverlap.AddDynamic(this, &AGrantAbilityActor::OnOverlapBegin);

}

void AGrantAbilityActor::GrantAbility(TWeakObjectPtr<AActor> OtherActor) const
{
	if(GetLocalRole() == ROLE_Authority)
	{
		if(const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
		{
			const TWeakObjectPtr<UProtelumAbilitySystemComponent> AbilitySystemComponentBase =
				Cast<UProtelumAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent());
			if(AbilitySystemComponentBase.IsValid())
			{
				AbilitySystemComponentBase->GiveAbility(FGameplayAbilitySpec(GameplayAbilityToAddOnOverlap, 1, INDEX_NONE));
			}
		}
	}
}

void AGrantAbilityActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GrantAbility(OtherActor);
}

// Called when the game starts or when spawned
void AGrantAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrantAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugString(GetWorld(),FVector(0, 0, 100),GameplayAbilityToAddOnOverlap->GetFName().ToString(), this, FColor::Green,DeltaTime);

}

