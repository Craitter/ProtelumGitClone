// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Debug/ClearAbilitySlotActor.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "ProtelumBaseClasses/ProtelumCharacterBase.h"

// Sets default values
AClearAbilitySlotActor::AClearAbilitySlotActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisibleActor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(VisibleActor);
	VisibleActor->OnComponentBeginOverlap.AddDynamic(this, &AClearAbilitySlotActor::OnOverlapBegin);

}

void AClearAbilitySlotActor::ClearAllAbilitySlots(AActor* OtherActor)
{
	const TWeakObjectPtr<AProtelumCharacterBase> PlayerCharacter = Cast<AProtelumCharacterBase>(OtherActor);
	if(PlayerCharacter.IsValid() && PlayerCharacter->GetAbilitySystemComponent())
	{
		for(const auto AbilitySlotTag : AbilitySlotTags)
		{
			const FGameplayAbilitySpec EmptySpec = FGameplayAbilitySpec(EmptyAbilityClass, 1, INDEX_NONE);
			PlayerCharacter->SetAbilitySlotToModify(AbilitySlotTag);
			PlayerCharacter->GetAbilitySystemComponent()->GiveAbility(EmptySpec);
		}

	}
}

void AClearAbilitySlotActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		ClearAllAbilitySlots(OtherActor);
	}
}

// Called when the game starts or when spawned
void AClearAbilitySlotActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClearAbilitySlotActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugString(GetWorld(),FVector(0, 0, 100), Name, this, FColor::Green,DeltaTime);
}

