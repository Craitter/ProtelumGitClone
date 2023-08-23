// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ItemActor.h"

#include "ActorComponents/ContainerComponent.h"
#include "ActorComponents/ItemTypes.h"

bool AItemActor::IsAvailableForInteraction_Implementation(UPrimitiveComponent* InteractionComponent) const
{
	return true;
}

FInteractMessageInformation AItemActor::GetInteractionMessageType_Implementation(
	UPrimitiveComponent* InteractionComponent) const
{
	return MessageInformation;
}

float AItemActor::GetInteractionDuration_Implementation(UPrimitiveComponent* InteractionComponent) const
{
	return 0.0f;
}

// void AItemActor::GetPostInteractSyncType_Implementation(bool& bShouldSync, EAbilityTaskNetSyncType& Type,
// 	UPrimitiveComponent* InteractionComponent) const
// {
// 	IProtelumInteractable::GetPostInteractSyncType_Implementation(bShouldSync, Type, InteractionComponent);
// }

void AItemActor::PostInteract_Implementation(AActor* InteractingActor, UPrimitiveComponent* InteractionComponent)
{
	UE_LOG(LogTemp, Warning , TEXT("%s %s() Interact"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__));
	if(InteractingActor == nullptr)
	{
		return;
	}
	const TWeakObjectPtr<UContainerComponent> Container = Cast<UContainerComponent>(InteractingActor->FindComponentByClass(UContainerComponent::StaticClass()));
	if(Container.IsValid())
	{
		Container->AddToContainer(ThisItem->Tag, StackSize);
		if(StackSize <= 0)
		{
			Destroy();
		}
	}
}

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	if(IsValid(ItemMesh))
	{
		SetRootComponent(ItemMesh);
	}
	MessageInformation.InteractMessageType = EInteractMessageType::IMT_Pickup;

	 ThisItem = &DevelopmentUPropertyItem;
}

void AItemActor::SetItem(FCollectableItem* InThisItem, int32 InStackSize)
{
	StackSize = InStackSize;
	ThisItem = InThisItem;
	if(ThisItem == nullptr)
	{
		Destroy();
		return;
	}
	if(IsValid(ItemMesh))
	{
		ItemMesh->SetStaticMesh(ThisItem->Mesh);
	}
	MessageInformation.InteractableName = ThisItem->Name;
	//Todo: do something with Rarity
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

