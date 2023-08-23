// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameOverlay/InventoryWidget.h"

bool UInventoryWidget::Initialize()
{
	return Super::Initialize();
}

UMenuAnchor* UInventoryWidget::GetInventoryAnchor()
{
	return InventoryAnchor;
}
