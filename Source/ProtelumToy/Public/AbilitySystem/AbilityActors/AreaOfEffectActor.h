// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AreaOfEffectActor.generated.h"

class UProtelumAbilitySystemComponent;
struct FGameplayEffectSpecHandle;
class UNiagaraComponent;
class UStaticMeshComponent;
UCLASS()
class PROTELUMTOY_API AAreaOfEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAreaOfEffectActor();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//GameplayEffects to Apply to ASCs that Overlaps with us
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn))
	TArray<FGameplayEffectSpecHandle> GameplayEffectsToApply;
	
	//If this is true we take the given Socket and Attach to it
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn))
	bool bShouldAttachToParent = false;
	//The Socket we attach to
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn))
	FName ParentSocket = NAME_None;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh = {nullptr};
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDecalComponent> Decal = {nullptr};
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraSystem = {nullptr};

	TMap<TWeakObjectPtr<UProtelumAbilitySystemComponent>, TArray<FActiveGameplayEffectHandle>> AffectedASCs;

	TWeakObjectPtr<UProtelumAbilitySystemComponent> InstigatorASC = {nullptr};

public:	


	

};
