// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "ClearAbilitySlotActor.generated.h"

class UGameplayAbility;

UCLASS()
class PROTELUMTOY_API AClearAbilitySlotActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClearAbilitySlotActor();


	void ClearAllAbilitySlots(AActor* OtherActor);
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> EmptyAbilityClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> VisibleActor = {nullptr};

	UPROPERTY(EditDefaultsOnly)
	FString Name;
	
	UPROPERTY(EditAnywhere, Meta = (Categories = "Ability.AbilitySlot.Dynamic"))
	TArray<FGameplayTag> AbilitySlotTags;

};
