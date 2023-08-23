// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ItemTypes.h"

void FCollectableItem::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	FTableRowBase::OnDataTableChanged(InDataTable, InRowName);
	FCollectableItem* RowItem = InDataTable->FindRow<FCollectableItem>(InRowName, TEXT("SetValuesForItemType"), true);
	if(RowItem != nullptr && RowItem->Type != EItemType::IT_None)
	{
		switch (RowItem->Type)
		{
			case EItemType::IT_Consumable:
				RowItem->MaxStackSize = MAX_STACK_CONSUMABLE;
				RowItem->bIsDroppable = true;
				break;
			case EItemType::IT_CraftingMaterial:
				RowItem->MaxStackSize = MAX_STACK_CRAFTING_MATERIAL;
				RowItem->bIsDroppable = true;
				break;

			case EItemType::IT_Ability_Shaman:
				RowItem->MaxStackSize = MAX_STACK_ABILITY_SHAMAN;
				RowItem->bIsDroppable = false;	
				break;

			case EItemType::IT_Ability_Human:
				RowItem->MaxStackSize = MAX_STACK_ABILITY_HUMAN;
				RowItem->bIsDroppable = false;
				break;

			case EItemType::IT_Artifact: 
				RowItem->MaxStackSize = MAX_STACK_ARTIFACT;
				RowItem->bIsDroppable = true;
				break;

			case EItemType::IT_Blueprint: 
				RowItem->MaxStackSize = MAX_STACK_BLUEPRINT;
				RowItem->bIsDroppable = true;
				break;
			default: ;
		}
	}
}
