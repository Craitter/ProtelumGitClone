// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Widgets/UtilityWidgets/MessageTypes.h"
#include "SaveGameSubsystem.generated.h"

class UProtelumSaveGame;
class UProtelumSaveArchive;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API USaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void LoadSaveGameArchive();

	
	bool CreateNewSaveGame(const FString& CharacterName, const FGameplayTag& CharacterType, EPromptMessageType& ErrorType);

	void DeleteSaveGame(const FString& SaveGameToDelete);

	bool IsActiveSaveGameLoaded() const;

	void LoadNextSaveGame();

	void LoadPreviousSaveGame();


	
	UFUNCTION()
	void OnAsyncArchiveLoadingFinished(const FString& SlotName, const int32 PlayerIndex, USaveGame* InArchive);
	UFUNCTION()
	void OnAsyncFindLastPlayedSaveGameFinished(const FString& SlotName, const int32 PlayerIndex, USaveGame* InLoadedSaveGame);
	FAsyncLoadGameFromSlotDelegate OnAsyncLoadGameFromSlotFinishedDelegate;
	uint32 AsyncLoadingIndex = 0;



	
	void GetCharacterName(FString& CharacterName) const;

	const FGameplayTag& GetCharacterType() const;

	int32 GetCharacterLevel() const;

	const TMap<FGameplayTag, uint32>* GetUnlockedAbilities() const;

	const FGameplayTagContainer* GetChosenAbilities() const;

	const FGameplayTagContainer* GetCraftedArtifacts() const;

	const FGameplayTagContainer* GetCollectedArtifactBlueprints() const;

	const TMap<FGameplayTag, uint32>* GetCollectedLoot() const;

	FString GetLastPlayedPlayerName() const;

	float GetCharacterTimePlayed() const;

	
protected:
	
private:

	bool CanCreateNewSaveGame(const FString& CharacterName, EPromptMessageType& ErrorType) const;

	bool IsEmptySaveGameSlotAvailable() const;

	static bool IsCharacterNameValid(const FString& NewSaveGameName);

	static bool IsCharacterNameSizeValid(uint32 CharacterNameSize);
	
	bool IsSaveGameNameAvailable(const FString& NewSaveGameName) const;

	int32 GetSaveGameArchiveIndexByName(const FString& SaveGameName) const;

	UPROPERTY()
	TObjectPtr<UProtelumSaveArchive> SaveGameArchive = {nullptr};

	UPROPERTY()
	TObjectPtr<UProtelumSaveGame> ActiveSaveGame = {nullptr};
};
