// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtelumBaseClasses/ProtelumGameInstance.h"

void UProtelumGameInstance::Host() const
{
	const TWeakObjectPtr<UWorld> World = GetWorld();
	if(!World.IsValid())
	{
		return;
	}
	World->ServerTravel("/Game/Levels/LVL_MultiplayerTest?listen");
}

void UProtelumGameInstance::Join(const FString& AddressToJoin) const
{
	const TWeakObjectPtr<APlayerController> PlayerController = GetFirstLocalPlayerController(GetWorld());
	if(!PlayerController.IsValid())
	{
		return;
	}
	PlayerController->ClientTravel(AddressToJoin, TRAVEL_Absolute);
	
}
