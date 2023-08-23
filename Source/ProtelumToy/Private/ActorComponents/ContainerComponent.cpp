// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ContainerComponent.h"

#include "ActorComponents/ItemTypes.h"
#include "Engine/CompositeDataTable.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/GameOverlay/ContainerGridWidget.h"


// Sets default values for this component's properties
UContainerComponent::UContainerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	
	SetIsReplicatedByDefault(true);
	
	
	ConstructorHelpers::FObjectFinder<UCompositeDataTable> DataTableBP(TEXT("/Game/Items/AllItems/ItemDataTableComposition"));
	if (DataTableBP.Object)
	{
		ItemDataTable = DataTableBP.Object;
	}

	//Todo: GetReference to data table
}

void UContainerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UContainerComponent, ItemContainer);	
	// DOREPLIFETIME(UContainerComponent, BasicAttackSlotsContainer);	
	// DOREPLIFETIME(UContainerComponent, AbilitySlotsContainer);	
	// DOREPLIFETIME(UContainerComponent, UltimateAbilitySlots);	
}

void UContainerComponent::InitializeContainerType(const EContainerTypes& InContainerType, int32 InContainerSize)
{	
	bool bOnlyChangeSize = false;
	if(ItemContainer.Type != EContainerTypes::CT_None)
	{
		bOnlyChangeSize = true;
	}
	ItemContainer.Type = InContainerType;

	ItemContainer.ContainerSize = InContainerSize;
	// if(ContainerType == EContainerType::CT_Inventory)
	// {
	// 	AbilitySlots = INVENTORY_BOUND_ABILITIES;
	// 	BasicAttackSlots = INVENTORY_BOUND_BASIC_ATTACKS;
	// 	UltimateAbilitySlots = INVENTORY_BOUND_ULTIMATES;
	// }
	// else
	// {
	// 	AbilitySlots = 0;
	// 	BasicAttackSlots = 0;
	// 	UltimateAbilitySlots = 0;
	// }
	// AbilitySlotsContainer.AddZeroed(AbilitySlots);
	// BasicAttackSlotsContainer.AddZeroed(BasicAttackSlots);
	// UltimateSlotsContainer.AddZeroed(UltimateAbilitySlots);
	switch (InContainerType)
	{
		case EContainerTypes::CT_Inventory:
			ItemContainer.ContainableItemTypes.Add(EItemType::IT_Artifact);
			ItemContainer.ContainableItemTypes.Add(EItemType::IT_Blueprint);
			ItemContainer.ContainableItemTypes.Add(EItemType::IT_Consumable);
			ItemContainer.ContainableItemTypes.Add(EItemType::IT_CraftingMaterial);
			break;
		case EContainerTypes::CT_Loot:
			ItemContainer.ContainableItemTypes.Add(EItemType::IT_Artifact); //Todo: Not sure yet
			ItemContainer.ContainableItemTypes.Add(EItemType::IT_Blueprint);
			ItemContainer.ContainableItemTypes.Add(EItemType::IT_Consumable);
			ItemContainer.ContainableItemTypes.Add(EItemType::IT_CraftingMaterial);
			break;
	
		case EContainerTypes::CT_Consumable:
			ItemContainer.ContainableItemTypes.Add(EItemType::IT_Consumable);
			break;
		
		case EContainerTypes::CT_CraftingMaterial:
			ItemContainer.ContainableItemTypes.Add(EItemType::IT_CraftingMaterial);
			break;
	
		case EContainerTypes::CT_AbilityShaman:
			ContainableItemTypes.Add(EItemType::IT_Ability_Shaman);
			break;
	
		case EContainerTypes::CT_AbilityHuman:
			ContainableItemTypes.Add(EItemType::IT_Ability_Human);
			break;
	
		case EContainerTypes::CT_Artifact:
			ContainableItemTypes.Add(EItemType::IT_Artifact);
			break;
	
		case EContainerTypes::CT_Blueprint:
			ContainableItemTypes.Add(EItemType::IT_Blueprint);
			break;
	
		default: ;
	}
}



bool UContainerComponent::AddToContainer(const FGameplayTag& ItemToAdd, int32& CountToAdd)
{
	if(ItemToAdd == FGameplayTag::EmptyTag)
	{
		return false;
	}
	const FCollectableItem* Item = GetItemReferenceByTag(ItemToAdd);
	if(Item == nullptr || !ItemContainer.ContainableItemTypes.Contains(Item->Type))
	{
		return false;
	}
	TArray<int32> EmptyIndexes;
	for(int32 i = 0; i < ItemContainer.ContainerContent.Num(); i++)
	{
		if(!ItemContainer.ContainerContent.IsValidIndex(i))
		{
			continue;
		}
		FContainerSlot& Slot = ItemContainer.ContainerContent[i];
		if(Slot.StackSize > 0)
		{
			TryAddItemToSlot(ItemToAdd, Item->MaxStackSize, Slot, CountToAdd);
			if(CountToAdd <= 0)
			{
				EmptyIndexes.Empty();
				break;
			}
		}
		else
		{
			EmptyIndexes.AddUnique(i);
		}
	}
	for(const int32& Index : EmptyIndexes)
	{
		if(!ItemContainer.ContainerContent.IsValidIndex(Index))
		{
			continue;
		}
		FContainerSlot& Slot = ItemContainer.ContainerContent[Index];
		Slot.ItemTag = ItemToAdd;
		TryAddItemToSlot(ItemToAdd, Item->MaxStackSize, Slot, CountToAdd);
		if(CountToAdd <= 0)
		{
			break;
		}
	}
	return true;
}

void UContainerComponent::SwapSlots(TWeakObjectPtr<UContainerComponent> SourceComponent, int32 SourceIndex,
	int32 DestinationIndex, bool& bDragSwappedItem)
{
	bDragSwappedItem = false;
	if(!SourceComponent.IsValid())
	{
		return;
	}
	// if(SourceComponent == this)
	// {
	// 	SwapSlotsIntern(SourceIndex, DestinationIndex, bDragSwappedItem);
	// }
	// else
	// {
	FContainerSlot* SourceSlot = SourceComponent->GetSlotByIndex(SourceIndex);
	FContainerSlot* DestinationSlot = GetSlotByIndex(DestinationIndex);
	if(SourceSlot == nullptr || DestinationSlot == nullptr)
	{
		return;
	}
	const FContainerSlot TempSlot = *DestinationSlot;
	*DestinationSlot = *SourceSlot;
	*SourceSlot = TempSlot;
	if(SourceSlot->ItemTag == FGameplayTag::EmptyTag)
	{
		return;
	}
	bDragSwappedItem = true;
	// }
}

void UContainerComponent::UpdateContainer()
{
	if(ContainerGridWidget.IsValid())
	{
		ContainerGridWidget->ShowContainerContent(this);
	}
}

bool UContainerComponent::IsItemAddable(EItemType ItemToAddType) const
{
	if(ContainableItemTypes.Contains(ItemToAddType))
	{
		return true;
	}
	return false;
}

FContainerType& UContainerComponent::GetItemContainer()
{
	return ItemContainer;
}

FContainerSlot* UContainerComponent::GetSlotByIndex(int32 SlotIndex)
{
	if(ItemContainer.ContainerContent.IsValidIndex(SlotIndex))
	{
		return &ItemContainer.ContainerContent[SlotIndex];
	}
	return nullptr;
}

void UContainerComponent::SetWidgetReference(UContainerGridWidget* ContainerGrid)
{
	ContainerGridWidget = ContainerGrid;
}

// int32 UContainerComponent::FindContainerSlot(const FCollectableItem* ItemToAdd)
// {
// }

bool UContainerComponent::RemoveItem(FCollectableItem* ItemToRemove)
{
	return true;
}

FCollectableItem* UContainerComponent::GetItemReferenceByTag(const FGameplayTag& ItemTag) const
{
	if(ItemDataTable == nullptr)
	{
		return nullptr;
	}
	// ItemDataTable->ForeachRow<FCollectableItem>(TEXT(""),[ItemTag]())
	TArray<FCollectableItem*> AllItems;
	ItemDataTable->GetAllRows(TEXT("Get All Rows"), AllItems);
	for(FCollectableItem* TempItem : AllItems)
	{
		if(TempItem != nullptr && TempItem->Tag == ItemTag)
		{
			return TempItem;
			
		}
	}
	return nullptr;
}


// Called when the game starts
void UContainerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UContainerComponent::TryAddItemToSlot(const FGameplayTag ItemToAdd, const int32 MaxStackSize,
                                           FContainerSlot& Slot, int32& CountToAdd)
{
	if(Slot.ItemTag == ItemToAdd)
	{
		while (CountToAdd > 0 && Slot.StackSize < MaxStackSize)
		{
			Slot.StackSize++;
			CountToAdd--;
		}
	}
}

void UContainerComponent::SwapSlotsIntern(int32 SourceIndex, int32 DestinationIndex, bool& bDragSwappedItem)
{
	
}


// Called every frame
void UContainerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UContainerComponent::OnRep_ContainerContent()
{
}

void UContainerComponent::Server_ReplaceSlotContent_Implementation(int32 SlotIndex, FGameplayTag ItemToAdd, int32 Count)
{
	
}

bool UContainerComponent::Server_ReplaceSlotContent_Validate(int32 SlotIndex, FGameplayTag ItemToAdd, int32 Count)
{
	return true;
}

