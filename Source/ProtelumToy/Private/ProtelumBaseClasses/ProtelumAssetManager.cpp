// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtelumBaseClasses/ProtelumAssetManager.h"

#include "AbilitySystemGlobals.h"

UProtelumAssetManager& UProtelumAssetManager::Get() 
{
	UProtelumAssetManager* Singleton = Cast<UProtelumAssetManager>(GEngine->AssetManager);

	if (Singleton)
	{
		return *Singleton;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be GDAssetManager!"));
		return *NewObject<UProtelumAssetManager>();	 // never calls this
	}
}


void UProtelumAssetManager::StartInitialLoading() 
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}