// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "ItemTypes.h"
#include "Components/ActorComponent.h"
#include "ContainerComponent.generated.h"

class UCompositeDataTable;
struct FGameplayTagCountContainer;
class UContainerGridWidget;
struct FGameplayTagContainer;
struct FCollectableItem;

UENUM()
enum class EContainerTypes
{
	CT_Inventory = 0				UMETA(DisplayName = "Inventory"),

	CT_Loot = 1						UMETA(DisplayName = "Loot"),

	CT_Consumable = 2				UMETA(DisplayName = "Consumables"),

	CT_CraftingMaterial = 3			UMETA(DisplayName = "CraftingMaterial"),

	CT_AbilityShaman = 4			UMETA(DisplayName = "ShamanAbility"),

	CT_AbilityHuman= 5				UMETA(DisplayName = "HumanAbility"),

	CT_Artifact = 6					UMETA(DisplayName = "Artifact"),

	CT_Blueprint = 7				UMETA(DisplayName = "Blueprint"),

	CT_None = 8						UMETA(Hidden),

};

USTRUCT()
struct FContainerSlot
{
	GENERATED_BODY()

	FGameplayTag ItemTag = FGameplayTag::EmptyTag;

	int32 StackSize = 0;
};

USTRUCT(BlueprintType)
struct FContainerType
{
	GENERATED_BODY()

	EContainerTypes Type = EContainerTypes::CT_None;

	int32 ContainerSize = -1; // means endless

	TArray<FContainerSlot> ContainerContent;

	TArray<EItemType> ContainableItemTypes;
	
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTELUMTOY_API UContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UContainerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InitializeContainerType(const EContainerTypes& InContainerType, int32 InContainerSize);
	
	bool AddToContainer(const FGameplayTag& ItemToAdd, int32& CountToAdd);

	void SwapSlots(TWeakObjectPtr<UContainerComponent> SourceComponent, int32 SourceIndex, int32 DestinationIndex, bool& bDragSwappedItem);
	
	void UpdateContainer();

	bool IsItemAddable(EItemType) const;
	
	// int32 FindContainerSlot(const FCollectableItem* ItemToAdd);
	UFUNCTION(BlueprintCallable)
	FContainerType& GetItemContainer();
	
	FContainerSlot* GetSlotByIndex(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void SetWidgetReference(UContainerGridWidget* ContainerGrid);
	
	bool RemoveItem(FCollectableItem* ItemToRemove);

	FCollectableItem* GetItemReferenceByTag(const FGameplayTag& ItemTag) const;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	static void TryAddItemToSlot(FGameplayTag ItemToAdd, int32 MaxStackSize, FContainerSlot& Slot, int32& CountToAdd);

	void SwapSlotsIntern(int32 SourceIndex, int32 DestinationIndex, bool& bDragSwappedItem);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	// void Test(const FName& Key, const FCollectableItem& Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ReplaceSlotContent(int32 SlotIndex, FGameplayTag ItemToAdd, int32 Count);

	UFUNCTION()
	void OnRep_ContainerContent();
	
	TWeakObjectPtr<UContainerGridWidget> ContainerGridWidget = {nullptr};
	
	// EContainerTypes ContainerType = EContainerTypes::CT_None;

	TArray<EItemType> ContainableItemTypes;

	UPROPERTY(Replicated)
	FContainerType ItemContainer;
	// UPROPERTY(Replicated)
	// TArray<FContainerSlot> BasicAttackSlotsContainer;
	// int32 BasicAttackSlots = 0;
	// FGameplayTagContainer Test;
	// // UPROPERTY(Replicated)
	// // TArray<FContainerSlot> AbilitySlotsContainer;
	// int32 AbilitySlots = 0;
	// //Todo: Maybe use a different TArray<Type*>
	// UPROPERTY(Replicated)
	// TArray<FContainerSlot> UltimateSlotsContainer;
	// int32 UltimateAbilitySlots = 0;

	TObjectPtr<UCompositeDataTable> ItemDataTable; // THIS NEXT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
};
