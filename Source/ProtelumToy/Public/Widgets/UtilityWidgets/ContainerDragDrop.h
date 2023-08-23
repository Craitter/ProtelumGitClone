// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ContainerDragDrop.generated.h"

struct FContainerSlot;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UContainerDragDrop : public UDragDropOperation
{
	GENERATED_BODY()
private:
	FContainerSlot* MovingSlot = {nullptr};

	int32 ContainerMovingSlotIndex = -1;
};
