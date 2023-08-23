// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "ProtelumSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UProtelumSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UProtelumSaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
	
	UPROPERTY(VisibleAnywhere, Category = "Essential")
	FString CharacterName;

	UPROPERTY(VisibleAnywhere, Category = "Essential")
	FGameplayTag CharacterType;

	UPROPERTY(VisibleAnywhere, Category = "Essential")
	uint32 Level;
	
	UPROPERTY(VisibleAnywhere, Category = "Essential")
	TMap<FGameplayTag, uint32> UnlockedAbilities;

	UPROPERTY(VisibleAnywhere, Category = "Essential")
	FGameplayTagContainer ChosenAbilities;

	UPROPERTY(VisibleAnywhere, Category = "Essential")
	FGameplayTagContainer CraftedArtifacts;
	
	UPROPERTY(VisibleAnywhere, Category = "Essential")
	FGameplayTagContainer CollectedArtifactBlueprints;

	UPROPERTY(VisibleAnywhere, Category = "Essential")
	TMap<FGameplayTag, uint32> CollectedLoot;

	// UPROPERTY(VisibleAnywhere, Category = "Essential")
	// FString CharacterName;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FString LastPlayedWith;
	
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bLastPlayed;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float TimePlayed; // In Hours and one decimal
};
