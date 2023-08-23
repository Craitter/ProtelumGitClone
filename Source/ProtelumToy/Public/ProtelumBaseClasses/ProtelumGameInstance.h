// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Widgets/UtilityWidgets/ServerTypes.h"
#include "ProtelumGameInstance.generated.h"

struct FHostSettings;
class IOnlineSubsystem;
class FOnlineSessionSearch;


/** Todo: GameInstance:
 * Todo: 1. Player Count work
 * Todo: 2. Host Privacy work 
 * Todo: 3. Is Server still Valid and not full
 * Todo: 4. Start Session (Stops ability to join)
 * Todo: 5. Voice Chat
 * Todo: 6. Chat
 * Todo: 7. Disconnect what to do
 * Todo: 8. Reconnect what to do
 * Todo: 9. Kick, Invite, mute a Player
 * Todo: 10. Join via invite and via SteamOverlay work
 * Todo: 11. Add Coop-Player as friend
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSearchResultsReadyDelegate, const TArray<FHostSettings>&)

UCLASS()
class PROTELUMTOY_API UProtelumGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UProtelumGameInstance();
	
	virtual void Init() override;

	void HostGame(const FHostSettings& InHostSettings);

	// UFUNCTION(exec)
	// void ShowPlayerCount() const;
	
	UFUNCTION(exec)
	void Host() const;

	UFUNCTION(exec)
	void FindSessions();

	FOnSearchResultsReadyDelegate* GetOnSearchResultsReadyDelegate();

	bool IsServerStillValid(int32 ServerIndex) const;
	bool IsServerPasswordProtected(int32 ServerIndex) const;
	bool IsServerPasswordCorrect(int32 ServerIndex, const FString& Password) const;
	void GetServerJoinRestrictions(int32 ServerIndex, int32& MinLevel, int32& MaxLevel, FGameplayTag& CharacterType) const;
	
	
	UFUNCTION(exec)
	void Join(const int32 ServerIndex) const;

	void OnCreateSessionComplete(FName SessionName, bool bSuccess) const;

	void OnDestroySessionComplete(FName SessionName, bool bSuccess) const;

	void OnFindSessionsComplete(bool bWasSuccessful) const;

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) const;

	void OnNetworkFailure(UWorld*, UNetDriver*,	ENetworkFailure::Type, const FString&) const;

	// void Login() const;
	
	void CreateSession() const;

	bool IsHostFriend(int32 ServerIndex) const;

	FString GetUsername() const;
protected:
	FOnlineSessionSearchResult* GetOnlineSessionSearchResultByIndex(int32 SearchResultIndex) const;
	
	IOnlineSessionPtr SessionInterface = {nullptr};
	TSharedPtr<FOnlineSessionSearch> SessionSearch = {nullptr};
	IOnlineSubsystem* OnlineSubsystem = {nullptr};
	
	
	IOnlineIdentityPtr IdentityInterface = {nullptr};
	IOnlineFriendsPtr FriendInterface = {nullptr};
	FHostSettings HostSettings;
	FOnSearchResultsReadyDelegate OnSearchResultsReadyDelegate;
};
