// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.h"
#include "AbilitySystem/Interface/ProtelumInteractable.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

struct FCollectableItem;

UCLASS()
class PROTELUMTOY_API AItemActor : public AActor, public IProtelumInteractable
{
public:

	
	virtual bool IsAvailableForInteraction_Implementation(UPrimitiveComponent* InteractionComponent) const override;
	
	virtual FInteractMessageInformation GetInteractionMessageType_Implementation(
		UPrimitiveComponent* InteractionComponent) const override;
	virtual float GetInteractionDuration_Implementation(UPrimitiveComponent* InteractionComponent) const override;
	
	// virtual void GetPostInteractSyncType_Implementation(bool& bShouldSync, EAbilityTaskNetSyncType& Type,
	// 	UPrimitiveComponent* InteractionComponent) const override;
	
	virtual void PostInteract_Implementation(AActor* InteractingActor, UPrimitiveComponent* InteractionComponent) override;

private:
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AItemActor();
	
	void SetItem(FCollectableItem* InThisItem, int32 InStackSize);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ItemMesh = {nullptr};

	UPROPERTY(EditDefaultsOnly)
	FCollectableItem DevelopmentUPropertyItem;

	
	FCollectableItem* ThisItem = {nullptr};
	UPROPERTY(EditDefaultsOnly)//Todo:Remove UpropertyMacro
	int32 StackSize = 1;
	UPROPERTY(EditDefaultsOnly)//Todo:Remove UpropertyMacro
	FInteractMessageInformation MessageInformation;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
