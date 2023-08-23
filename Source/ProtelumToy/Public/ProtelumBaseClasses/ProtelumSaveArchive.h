// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ProtelumSaveArchive.generated.h"

/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UProtelumSaveArchive : public USaveGame
{
	GENERATED_BODY()
public:
	UProtelumSaveArchive();
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FString> SaveGameNames;
};
