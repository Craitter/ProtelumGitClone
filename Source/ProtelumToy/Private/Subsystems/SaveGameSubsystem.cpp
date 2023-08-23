// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/SaveGameSubsystem.h"

#include "ProtelumBaseClasses/ProtelumSaveArchive.h"
#include "ProtelumBaseClasses/ProtelumSaveGame.h"
#include "ProtelumToy/ProtelumToy.h"
#include "Widgets/UtilityWidgets/MessageTypes.h"

constexpr int32 GMax_Save_Slots = 50;
constexpr int32 GMax_CharacterName_Length = 15;
constexpr int32 GMin_CharacterName_Length = 3;

void USaveGameSubsystem::LoadSaveGameArchive()
{
	if(UGameplayStatics::DoesSaveGameExist(DEFAULT_SAVE_SLOT, 0))
	{
		OnAsyncLoadGameFromSlotFinishedDelegate.BindUObject(this, &USaveGameSubsystem::OnAsyncArchiveLoadingFinished);
		UGameplayStatics::AsyncLoadGameFromSlot(DEFAULT_SAVE_SLOT, 0, OnAsyncLoadGameFromSlotFinishedDelegate);
		return;
	}
	SaveGameArchive = Cast<UProtelumSaveArchive>(UGameplayStatics::CreateSaveGameObject(UProtelumSaveArchive::StaticClass()));
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameArchive, DEFAULT_SAVE_SLOT, 0);
}

bool USaveGameSubsystem::CreateNewSaveGame(const FString& CharacterName, const FGameplayTag& CharacterType, EPromptMessageType& ErrorType)
{
	if(SaveGameArchive == nullptr || !CanCreateNewSaveGame(CharacterName, ErrorType))
	{
		return false;
	}
	SaveGameArchive->SaveGameNames.Add(CharacterName);
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameArchive, DEFAULT_SAVE_SLOT, 0);
	
	ActiveSaveGame = Cast<UProtelumSaveGame>(UGameplayStatics::CreateSaveGameObject(UProtelumSaveGame::StaticClass()));
	if(ActiveSaveGame != nullptr)
	{
		ActiveSaveGame->SaveSlotName = CharacterName;
		ActiveSaveGame->CharacterName = CharacterName;
		ActiveSaveGame->CharacterType = CharacterType;
		UGameplayStatics::AsyncSaveGameToSlot(ActiveSaveGame, CharacterName, 0);
	}
	return true;
}

void USaveGameSubsystem::DeleteSaveGame(const FString& SaveGameToDelete)
{
	if(ActiveSaveGame != nullptr)
	{
		if(ActiveSaveGame->SaveSlotName == SaveGameToDelete)
		{
			ActiveSaveGame = nullptr;
		}
	}
	if(!UGameplayStatics::DeleteGameInSlot(SaveGameToDelete, 0))
	{
		UE_LOG(LogTemp, Error , TEXT("%s() DELETIONFAILED"), *FString(__FUNCTION__));
		return;
	}
	if(SaveGameArchive != nullptr)
	{
		const int32 SaveGameArchiveIndex = GetSaveGameArchiveIndexByName(SaveGameToDelete);
		if(SaveGameArchiveIndex != -1)
		{
			SaveGameArchive->SaveGameNames.RemoveAt(SaveGameArchiveIndex);
			UGameplayStatics::AsyncSaveGameToSlot(SaveGameArchive, DEFAULT_SAVE_SLOT, 0);
		}
	}
}

bool USaveGameSubsystem::IsActiveSaveGameLoaded() const
{
	if(ActiveSaveGame != nullptr)
	{
		return true;
	}
	return false;
}

void USaveGameSubsystem::LoadNextSaveGame()
{
	if(ActiveSaveGame == nullptr || SaveGameArchive == nullptr)
	{
		return;
	}
	ActiveSaveGame->bLastPlayed = false;
	int32 CurrentSaveGameArchiveIndex = GetSaveGameArchiveIndexByName(ActiveSaveGame->SaveSlotName);
	if(CurrentSaveGameArchiveIndex == -1)
	{
		return;
	}
	CurrentSaveGameArchiveIndex++;
	if(CurrentSaveGameArchiveIndex >= SaveGameArchive->SaveGameNames.Num())
	{
		CurrentSaveGameArchiveIndex = 0;
	}
	const FString& SaveGameNameToLoad = SaveGameArchive->SaveGameNames[CurrentSaveGameArchiveIndex];
	ActiveSaveGame = Cast<UProtelumSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameNameToLoad, 0));
	if(ActiveSaveGame != nullptr)
	{
		ActiveSaveGame->bLastPlayed = true;
	}
}

void USaveGameSubsystem::LoadPreviousSaveGame()
{
	if(ActiveSaveGame == nullptr || SaveGameArchive == nullptr)
	{
		return;
	}
	ActiveSaveGame->bLastPlayed = false;
	int32 CurrentSaveGameArchiveIndex = GetSaveGameArchiveIndexByName(ActiveSaveGame->SaveSlotName);
	if(CurrentSaveGameArchiveIndex == -1)
	{
		return;
	}
	CurrentSaveGameArchiveIndex--;
	if(CurrentSaveGameArchiveIndex < 0)
	{
		CurrentSaveGameArchiveIndex = SaveGameArchive->SaveGameNames.Num() - 1;
	}
	const FString& SaveGameNameToLoad = SaveGameArchive->SaveGameNames[CurrentSaveGameArchiveIndex];
	ActiveSaveGame = Cast<UProtelumSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameNameToLoad, 0));
	if(ActiveSaveGame != nullptr)
	{
		ActiveSaveGame->bLastPlayed = true;
	}
}

void USaveGameSubsystem::OnAsyncArchiveLoadingFinished(const FString& SlotName, const int32 PlayerIndex,
                                                       USaveGame* InArchive)
{
	OnAsyncLoadGameFromSlotFinishedDelegate.Unbind();
	SaveGameArchive = Cast<UProtelumSaveArchive>(InArchive);
	if(SaveGameArchive == nullptr)
	{
		UE_LOG(LogTemp, Error , TEXT("%s() ARCHIVE LOADING FAILED!"), *FString(__FUNCTION__));
		return;
	}
	OnAsyncLoadGameFromSlotFinishedDelegate.BindUObject(this, &USaveGameSubsystem::OnAsyncFindLastPlayedSaveGameFinished);
	for(const FString& SaveGameName : SaveGameArchive->SaveGameNames)
	{
		AsyncLoadingIndex++;
		UGameplayStatics::AsyncLoadGameFromSlot(SaveGameName, 0, OnAsyncLoadGameFromSlotFinishedDelegate);
	}
}

void USaveGameSubsystem::OnAsyncFindLastPlayedSaveGameFinished(const FString& SlotName, const int32 PlayerIndex,
	USaveGame* InLoadedSaveGame)
{
	const TObjectPtr<UProtelumSaveGame> LoadedSaveGame = Cast<UProtelumSaveGame>(InLoadedSaveGame);
	if(LoadedSaveGame != nullptr && LoadedSaveGame->bLastPlayed)
	{
		ActiveSaveGame = LoadedSaveGame;
		OnAsyncLoadGameFromSlotFinishedDelegate.Unbind();
	}
	AsyncLoadingIndex--;
	if(AsyncLoadingIndex == 0)
	{
		OnAsyncLoadGameFromSlotFinishedDelegate.Unbind();
	}
}

void USaveGameSubsystem::GetCharacterName(FString& CharacterName) const
{
	if(ActiveSaveGame != nullptr)
	{
		CharacterName = ActiveSaveGame->CharacterName;
	}
}

const FGameplayTag& USaveGameSubsystem::GetCharacterType() const
{
	if(ActiveSaveGame != nullptr)
	{
		return ActiveSaveGame->CharacterType;
	}
	return FGameplayTag::EmptyTag;
}

int32 USaveGameSubsystem::GetCharacterLevel() const
{
	if(ActiveSaveGame != nullptr)
	{
		return ActiveSaveGame->Level;
	}
	return -1;
}

const TMap<FGameplayTag, uint32>* USaveGameSubsystem::GetUnlockedAbilities() const
{
	if(ActiveSaveGame != nullptr)
	{
		return &ActiveSaveGame->UnlockedAbilities;
	}
	return nullptr;
}

const FGameplayTagContainer* USaveGameSubsystem::GetChosenAbilities() const
{
	if(ActiveSaveGame != nullptr)
	{
		return &ActiveSaveGame->ChosenAbilities;
	}
	return nullptr;
}

const FGameplayTagContainer* USaveGameSubsystem::GetCraftedArtifacts() const
{
	if(ActiveSaveGame != nullptr)
	{
		return &ActiveSaveGame->CraftedArtifacts;
	}
	return nullptr;
}

const FGameplayTagContainer* USaveGameSubsystem::GetCollectedArtifactBlueprints() const
{
	if(ActiveSaveGame != nullptr)
	{
		return &ActiveSaveGame->CollectedArtifactBlueprints;
	}
	return nullptr;
}

const TMap<FGameplayTag, uint32>* USaveGameSubsystem::GetCollectedLoot() const
{
	if(ActiveSaveGame != nullptr)
	{
		return &ActiveSaveGame->CollectedLoot;
	}
	return nullptr;
}

FString USaveGameSubsystem::GetLastPlayedPlayerName() const
{
	if(ActiveSaveGame != nullptr)
	{
		return ActiveSaveGame->LastPlayedWith;
	}
	return FString(); //Todo: Find way to localize this with N/A
}

float USaveGameSubsystem::GetCharacterTimePlayed() const
{
	if(ActiveSaveGame != nullptr)
	{
		return ActiveSaveGame->TimePlayed;
	}
	return -1.0f;
}

bool USaveGameSubsystem::CanCreateNewSaveGame(const FString& CharacterName, EPromptMessageType& ErrorType) const
{
	bool bCanCreate = true;
	if(!IsEmptySaveGameSlotAvailable())
	{
		ErrorType = EPromptMessageType::MaxCharacterCount;
		bCanCreate = false;
	}
	else if(!IsCharacterNameValid(CharacterName))
	{
		ErrorType = EPromptMessageType::NoSymbols;
		bCanCreate = false;
	}
	else if(!IsCharacterNameSizeValid(CharacterName.Len()))
	{
		ErrorType = EPromptMessageType::InvalidNameSize;
		bCanCreate = false;
	}
	else if(!IsSaveGameNameAvailable(CharacterName))
	{
		ErrorType = EPromptMessageType::DuplicatedName;
		bCanCreate = false;
	}
	return bCanCreate;
}

bool USaveGameSubsystem::IsEmptySaveGameSlotAvailable() const
{
	if(SaveGameArchive == nullptr || SaveGameArchive->SaveGameNames.Num() >= GMax_Save_Slots)
	{
		return false;
	}
	return true;
}

bool USaveGameSubsystem::IsCharacterNameValid(const FString& NewSaveGameName)
{
	const FString AllowedCharacters = VALID_SAVEDDIRSUFFIX_CHARACTERS;
	for(int32 i = 0; i < NewSaveGameName.Len(); i++)// i = i + 1
	{
		if(!AllowedCharacters.Contains(NewSaveGameName.Mid(i, 1)))
		{
			return false;
		}	
	}
	return true;
}

bool USaveGameSubsystem::IsCharacterNameSizeValid(uint32 CharacterNameSize)
{
	if(CharacterNameSize > GMax_CharacterName_Length || CharacterNameSize < GMin_CharacterName_Length)
	{
		return false;
	}
	return true;
}

bool USaveGameSubsystem::IsSaveGameNameAvailable(const FString& NewSaveGameName) const
{
	if(SaveGameArchive == nullptr)
	{
		return false;
	}
	for(const auto& SaveGameName : SaveGameArchive->SaveGameNames)
	{
		if(SaveGameName.Equals(NewSaveGameName, ESearchCase::IgnoreCase))
		{
			return false;
		}
	}
	return true;
}

int32 USaveGameSubsystem::GetSaveGameArchiveIndexByName(const FString& SaveGameName) const
{
	for(int32 i = 0; i < SaveGameArchive->SaveGameNames.Num(); i++)
	{
		if(SaveGameArchive->SaveGameNames[i] == SaveGameName)
		{
			return i;
		}
	}
	return -1;
}
