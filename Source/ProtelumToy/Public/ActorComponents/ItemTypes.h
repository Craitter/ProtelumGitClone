// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "ProtelumToy/ProtelumToy.h"
#include "ItemTypes.generated.h"


class AItemActor;
//Todo: make sure the right things are exposed
UENUM()
enum class EItemType
{
	IT_Consumable = 0			UMETA(DisplayName = "Consumable"),

	IT_CraftingMaterial = 1		UMETA(DisplayName = "CraftingMaterial"),

	IT_Ability_Shaman = 2		UMETA(DisplayName = "ShamanAbility"),

	IT_Ability_Human = 3		UMETA(DisplayName = "HumanAbility"),

	IT_Artifact = 4				UMETA(DisplayName = "Artifact"),

	IT_Blueprint = 5			UMETA(DisplayName = "Blueprint"),

	IT_None = 6					UMETA(Hidden),
};

UENUM()
enum class EItemRarity
{
	IR_Common = 0		UMETA(DisplayName = "Common"),

	IR_Uncommon = 1		UMETA(DisplayName = "Uncommon"),

	IR_Rare = 2			UMETA(DisplayName = "Rare"),

	IR_VeryRare = 3		UMETA(DisplayName = "VeryRare"),

	IR_Legendary = 4	UMETA(DisplayName = "Legendary"),

	IR_None = 5			UMETA(Hidden),
};

USTRUCT(BlueprintType)
struct FCollectableItem : public FTableRowBase
{	
	GENERATED_BODY()

	//This will update automatically when updating a RowMember First Set
	//the RowName then the Rest so there is no update issue
	UPROPERTY(EditAnywhere)
	FText Name = FText::GetEmpty();
	
	UPROPERTY(EditAnywhere)
	FText Description = FText::GetEmpty();
	
	UPROPERTY(EditAnywhere)
	UTexture2D* Thumbnail = nullptr;
	
	UPROPERTY(EditAnywhere)
	int32 MaxStackSize = 0;
	
	UPROPERTY(EditAnywhere)
	EItemType Type = EItemType::IT_None;

	UPROPERTY(EditAnywhere)
	EItemRarity Rarity = EItemRarity::IR_None;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItemActor> ItemActor = {nullptr};

	UPROPERTY(VisibleAnywhere)
	bool bIsDroppable = true;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Mesh = {nullptr};

	//TSubclassOf<> ItemClass;
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
};

USTRUCT(BlueprintType)
struct FCollectableItem_Consumable : public FCollectableItem
{
	GENERATED_BODY()
	FCollectableItem_Consumable()
	{
		Type = EItemType::IT_Consumable;
		MaxStackSize = MAX_STACK_CONSUMABLE;
	}
};

USTRUCT(BlueprintType)
struct FCollectableItem_CraftingMaterial : public FCollectableItem
{
	GENERATED_BODY()
	FCollectableItem_CraftingMaterial()
	{
		Type = EItemType::IT_CraftingMaterial;
		MaxStackSize = MAX_STACK_CRAFTING_MATERIAL;
	}
};

USTRUCT(BlueprintType)
struct FCollectableItem_HumanAbility : public FCollectableItem
{
	GENERATED_BODY()
	FCollectableItem_HumanAbility()
	{
		Type = EItemType::IT_Ability_Human;
		MaxStackSize = MAX_STACK_ABILITY_HUMAN;
		bIsDroppable = false;
	}
};

USTRUCT(BlueprintType)
struct FCollectableItem_ShamanAbility : public FCollectableItem
{
	GENERATED_BODY()
	FCollectableItem_ShamanAbility()
	{
		Type = EItemType::IT_Ability_Shaman;
		MaxStackSize = MAX_STACK_ABILITY_SHAMAN;
		bIsDroppable = false;
	}
};

USTRUCT(BlueprintType)
struct FCollectableItem_Artifact : public FCollectableItem
{
	GENERATED_BODY()
	FCollectableItem_Artifact()
	{
		Type = EItemType::IT_Artifact;
		MaxStackSize = MAX_STACK_ARTIFACT;
		bIsDroppable = false;
	}
};

USTRUCT(BlueprintType)
struct FCollectableItem_Blueprint : public FCollectableItem
{
	GENERATED_BODY()
	FCollectableItem_Blueprint()
	{
		Type = EItemType::IT_Blueprint;
		MaxStackSize = MAX_STACK_BLUEPRINT;
	}
};
