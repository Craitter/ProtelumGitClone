// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Debug/LevelDownAbilityActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "ProtelumBaseClasses/ProtelumCharacterBase.h"

// Sets default values
ALevelDownAbilityActor::ALevelDownAbilityActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisibleActor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(VisibleActor);
	VisibleActor->OnComponentBeginOverlap.AddDynamic(this, &ALevelDownAbilityActor::OnOverlapBegin);
}

void ALevelDownAbilityActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LevelDownAbility(OtherActor);
}

void ALevelDownAbilityActor::LevelDownAbility(TObjectPtr<AActor> ActorToLevel) const
{
	const TWeakObjectPtr<AProtelumCharacterBase> CharBase = Cast<AProtelumCharacterBase>(ActorToLevel);
	if(CharBase.IsValid())
	{
			CharBase->DecreaseAbilityLevelBySlot(AbilitySlot);
	}
}

// Called when the game starts or when spawned
void ALevelDownAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelDownAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugString(GetWorld(),FVector(0, 0, 100), AbilitySlot.ToString() + "LevelDown", this, FColor::Green,DeltaTime);

}

