// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ProtelumToy/ProtelumToy.h"
#include "ServerTypes.generated.h"


#define SESSION_KEY_NAME							TEXT("ServerName")
#define SESSION_KEY_HOST							TEXT("Host")
#define SESSION_KEY_PASSWORD						TEXT("Password")
#define SESSION_KEY_CHARACTER_TYPE					TEXT("RequiredType")
#define SESSION_KEY_SERVER_MODE						TEXT("Mode")
#define SESSION_KEY_LEVEL_MIN						TEXT("MinLevel")
#define SESSION_KEY_LEVEL_MAX						TEXT("MaxLevel")
#define SESSION_KEY_CHAT_VOICE						TEXT("VoiceChat")



USTRUCT()
struct FServerListEntryData
{
	GENERATED_BODY()

	FText ServerName = FText::GetEmpty();

	FText HostName = FText::GetEmpty();

	TWeakObjectPtr<UTexture2D> PasswordLockedImage = {nullptr};

	FText RequiredType = FText::GetEmpty();

	FText ServerMode = FText::GetEmpty();

	FText LevelRange = FText::GetEmpty();

	TWeakObjectPtr<UTexture2D> VoiceChatImage = {nullptr};

	int32 ServerIndex = -1;
};

USTRUCT()
struct FServerSearchSettings
{
	GENERATED_BODY()

	bool bShouldBeFiltered = false;

	FGameplayTag SearchPrivacyFilter = FGameplayTag::EmptyTag;

	FGameplayTag RequiredTypeFilter = FGameplayTag::EmptyTag;

	FGameplayTag ServerModeFilter = FGameplayTag::EmptyTag;

	int32 AllowedLevelFilter = 1;

	bool bIsVoiceChatEnabledFilter = true;
};

USTRUCT()
struct FHostSettings
{

	GENERATED_BODY()
	
	FString ServerName = FString();

	FString HostName = FString();

	FString Password = FString();

	FGameplayTag ServerPrivacy;

	FGameplayTag RequiredType = FGameplayTag::EmptyTag;

	FGameplayTag ServerMode;

	int32 MinLevel = CHARACTER_LEVEL_MIN;

	int32 MaxLevel = CHARACTER_LEVEL_MAX;

	bool bIsVoiceChatEnabled = true;

	int32 Index;
};



