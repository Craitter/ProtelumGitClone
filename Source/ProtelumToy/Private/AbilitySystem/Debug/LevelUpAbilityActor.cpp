// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Debug/LevelUpAbilityActor.h"

#include "ProtelumBaseClasses/ProtelumCharacterBase.h"

// Sets default values
ALevelUpAbilityActor::ALevelUpAbilityActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VisibleActor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(VisibleActor);
	VisibleActor->OnComponentBeginOverlap.AddDynamic(this, &ALevelUpAbilityActor::OnOverlapBegin);
}

void ALevelUpAbilityActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LevelUpAbility(OtherActor);
}

void ALevelUpAbilityActor::LevelUpAbility(TObjectPtr<AActor> ActorToLevel) const
{
	const TWeakObjectPtr<AProtelumCharacterBase> CharBase = Cast<AProtelumCharacterBase>(ActorToLevel);
	if(CharBase.IsValid())
	{
		CharBase->IncreaseAbilityLevelBySlot(AbilitySlot);
	}
}

// Called when the game starts or when spawned
void ALevelUpAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelUpAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugString(GetWorld(),FVector(0, 0, 100), AbilitySlot.ToString() + "LevelUp", this, FColor::Green,DeltaTime);

}

