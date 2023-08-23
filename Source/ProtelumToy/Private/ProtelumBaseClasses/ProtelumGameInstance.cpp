// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtelumBaseClasses/ProtelumGameInstance.h"


#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/SaveGameSubsystem.h"
#include "Widgets/UtilityWidgets/ServerTypes.h"

const static FName SESSION_NAME = NAME_GameSession;

UProtelumGameInstance::UProtelumGameInstance()
{
}


void UProtelumGameInstance::Init()
{	
	Super::Init();
	
	// FOnlineAccountCredentials
	
	OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning , TEXT(" %s() OnlineSubsystemName %s"), *FString(__FUNCTION__), *OnlineSubsystem->GetSubsystemName().ToString());
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UProtelumGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UProtelumGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UProtelumGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UProtelumGameInstance::OnJoinSessionComplete);
		}
		IdentityInterface = OnlineSubsystem->GetIdentityInterface();
		if(IdentityInterface.IsValid())
		{
			// IdentityInterface->OnLoginCompleteDelegates->AddUObject(this, &UProtelumGameInstance::OnLoginComplete);
			// FString Token = IdentityInterface->GetAuthToken(0);
			// Login();
			
			// ISteamUser::GetAuthSessionTicket(NULL, )
			// EOS_Auth_Login(EOS_EExternalCredentialType::EOS_ECT_STEAM_SESSION_TICKET,)
			// SteamUser()->GetAuthSessionTicket()
			// EOS_ECT_STEAM_SESSION_TICKET
			// SteamNetworkingIdentity::
		}
		FriendInterface = OnlineSubsystem->GetFriendsInterface();
		if(FriendInterface.IsValid())
		{
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Error , TEXT("%s() No OnlineSubsystemFound "), *FString(__FUNCTION__));
	}

	check(GetEngine())
	GEngine->OnNetworkFailure().AddUObject(this, &UProtelumGameInstance::OnNetworkFailure);

	const TWeakObjectPtr<USaveGameSubsystem> SaveGameSubsystem = GetSubsystem<USaveGameSubsystem>();
	if(SaveGameSubsystem.IsValid())
	{
		SaveGameSubsystem->LoadSaveGameArchive();
	}
}

void UProtelumGameInstance::HostGame(const FHostSettings& InHostSettings)
{
	HostSettings = InHostSettings;
	if(SessionInterface.IsValid())
	{
		// CustomServerName = MemberCustomServerName;
		const FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if(ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
	UE_LOG(LogTemp, Warning , TEXT("%s() now we would host"), *FString(__FUNCTION__));
}

void UProtelumGameInstance::Host() const
{
	if(SessionInterface.IsValid())
	{
		// CustomServerName = MemberCustomServerName;
		const FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if(ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UProtelumGameInstance::FindSessions()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if(SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = false;
		if(OnlineSubsystem->GetSubsystemName() == TEXT("NULL"))
		{
			SessionSearch->bIsLanQuery = true;
		}
		SessionSearch->MaxSearchResults = 1000;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		// SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

FOnSearchResultsReadyDelegate* UProtelumGameInstance::GetOnSearchResultsReadyDelegate()
{
	return &OnSearchResultsReadyDelegate;
}

bool UProtelumGameInstance::IsServerStillValid(int32 ServerIndex) const
{
	// const FOnlineSessionSearchResult* Result = GetOnlineSessionSearchResultByIndex(ServerIndex);
	// if(Result != nullptr)
	// {
	// 	if(Result->IsValid()) //todo: Check if PlayerCount is not full
	// 	{
	// 		return true;
	// 	}
	// }
	// return false;
	return true;
}

bool UProtelumGameInstance::IsServerPasswordProtected(int32 ServerIndex) const
{
	const FOnlineSessionSearchResult* Result = GetOnlineSessionSearchResultByIndex(ServerIndex);
	if(Result == nullptr)
	{
		return false;
	}
	FString Password;
	Result->Session.SessionSettings.Get<FString>(SESSION_KEY_PASSWORD, Password);
	if(!Password.IsEmpty())
	{
		return true;
	}
	return false;
}

bool UProtelumGameInstance::IsServerPasswordCorrect(int32 ServerIndex, const FString& Password) const
{
	const FOnlineSessionSearchResult* Result = GetOnlineSessionSearchResultByIndex(ServerIndex);
	if(Result == nullptr)
	{
		return false;
	}
	FString ServerPassword;
	Result->Session.SessionSettings.Get<FString>(SESSION_KEY_PASSWORD, ServerPassword);
	if(ServerPassword.Equals(Password))
	{
		return true;
	}
	return false;
}

void UProtelumGameInstance::GetServerJoinRestrictions(int32 ServerIndex, int32& MinLevel, int32& MaxLevel,
	FGameplayTag& CharacterType) const
{
	const FOnlineSessionSearchResult* Result = GetOnlineSessionSearchResultByIndex(ServerIndex);
	if(Result == nullptr)
	{
		return;
	}
	FString TempTypeString;
	Result->Session.SessionSettings.Get<int32>(SESSION_KEY_LEVEL_MIN, MinLevel);
	Result->Session.SessionSettings.Get<int32>(SESSION_KEY_LEVEL_MAX, MaxLevel);
	Result->Session.SessionSettings.Get<FString>(SESSION_KEY_CHARACTER_TYPE, TempTypeString);
	CharacterType = FGameplayTag::RequestGameplayTag(*TempTypeString);
}

void UProtelumGameInstance::Join(const int32 ServerIndex) const
{
	const FOnlineSessionSearchResult* Result = GetOnlineSessionSearchResultByIndex(ServerIndex);
	if(Result == nullptr)
	{
		return;
	}
	UE_LOG(LogTemp, Warning , TEXT("%s() Join"), *FString(__FUNCTION__));
	SessionInterface->JoinSession(0, SESSION_NAME, *Result);
}

void UProtelumGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess) const
{
	if(!bSuccess)
	{
		UE_LOG(LogTemp, Warning , TEXT("%s() Creating Session %s failed"), *FString(__FUNCTION__), *SessionName.ToString());
	}
	const TWeakObjectPtr<UWorld> World = GetWorld();
	if(!World.IsValid())
	{
		return;
	}
	World->ServerTravel("/Game/Levels/LVL_MultiplayerTest?listen");
}

void UProtelumGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess) const
{
	if(bSuccess)
	{
		CreateSession();
	}
}

void UProtelumGameInstance::OnFindSessionsComplete(bool bWasSuccessful) const
{
	// UE_LOG(LogTemp, Warning , TEXT("%s() find Sessions Complete"), *FString(__FUNCTION__));
	if(bWasSuccessful && SessionSearch.IsValid())
	{
		TArray<FHostSettings> FoundSessionsHostData;
		// FHostSettings TempInHostSettings;
		// TempInHostSettings.ServerName = TEXT("TEXT123");
		// TempInHostSettings.ServerMode = FGameplayTag::RequestGameplayTag("Mode.Normal");
		// TempInHostSettings.RequiredType = FGameplayTag::RequestGameplayTag("Shaman");
		// TempInHostSettings.MinLevel = 10;
		// TempInHostSettings.MaxLevel = 20;
		// TempInHostSettings.bIsVoiceChatEnabled = false;
		// FoundSessionsHostData.Add(TempInHostSettings);
		for(int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			const FOnlineSessionSearchResult* Result = GetOnlineSessionSearchResultByIndex(i);
			if(Result != nullptr && Result->IsValid())
			{
				FString TempCharacterString;
				FString TempServerModeString;
				
				FHostSettings HostData;
				Result->Session.SessionSettings.Get<FString>(SESSION_KEY_NAME, HostData.ServerName);
				Result->Session.SessionSettings.Get<FString>(SESSION_KEY_HOST, HostData.HostName);
				Result->Session.SessionSettings.Get<FString>(SESSION_KEY_PASSWORD, HostData.Password);
				Result->Session.SessionSettings.Get<FString>(SESSION_KEY_CHARACTER_TYPE, TempCharacterString);
				Result->Session.SessionSettings.Get<FString>(SESSION_KEY_SERVER_MODE, TempServerModeString);
				Result->Session.SessionSettings.Get<int32>(SESSION_KEY_LEVEL_MIN, HostData.MinLevel);
				Result->Session.SessionSettings.Get<int32>(SESSION_KEY_LEVEL_MAX, HostData.MaxLevel);
				Result->Session.SessionSettings.Get<bool>(SESSION_KEY_CHAT_VOICE, HostData.bIsVoiceChatEnabled);
				HostData.RequiredType = FGameplayTag::RequestGameplayTag(*TempCharacterString);
				HostData.ServerMode = FGameplayTag::RequestGameplayTag(*TempServerModeString);
				HostData.Index = i;
				FoundSessionsHostData.Add(HostData);
			}
		}
		OnSearchResultsReadyDelegate.Broadcast(FoundSessionsHostData);
	}
}

void UProtelumGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) const
{
	if(SessionInterface.IsValid())
	{
		FString SessionAddress;
		if(!SessionInterface->GetResolvedConnectString(SessionName, SessionAddress))
		{
			UE_LOG(LogTemp, Warning , TEXT("%s connextString Invalid"), *FString(__FUNCTION__));
			return;
		}
		const TWeakObjectPtr<APlayerController> PlayerController = GetFirstLocalPlayerController();
		if(!PlayerController.IsValid())
		{
			return;
		}
		UE_LOG(LogTemp, Warning , TEXT("%s() Session Address %s"), *FString(__FUNCTION__), *SessionAddress);
		PlayerController->ClientTravel(SessionAddress, TRAVEL_Absolute);
	}
}

void UProtelumGameInstance::OnNetworkFailure(UWorld*, UNetDriver*, ENetworkFailure::Type, const FString&) const
{
	UE_LOG(LogTemp, Warning , TEXT("%s() Network Error"), *FString(__FUNCTION__));
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel("/Game/Levels/LVL_MainMenuMap", TRAVEL_Absolute);
}

// void UProtelumGameInstance::Login() const
// {
// 	if(OnlineSubsystem != nullptr)
// 	{
// 		
// 		if(IdentityInterface.IsValid())
// 		{
// 			FOnlineAccountCredentials AccountCredentials;
// 			AccountCredentials.Id = FString("");
// 			AccountCredentials.Token = FString("");
// 			AccountCredentials.Type = FString("AccountPortal");
// 			// FString Token = IdentityInterface->
// 			// UE_LOG(LogTemp, Warning , TEXT("%s() Token %s"), *FString(__FUNCTION__), *Token);
// 			IdentityInterface->Login(0, AccountCredentials);
// 		}
// 	}
// }

void UProtelumGameInstance::CreateSession() const
{
	if(SessionInterface.IsValid())
	{
		
		
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bAllowInvites = true;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bAllowJoinViaPresence = true;
		SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
		//Todo: Make Host Server Privacy work
		// if(HostSettings.ServerPrivacy == FGameplayTag::RequestGameplayTag("Server.Privacy.FriendsOnly"))
		// {
		// 	SessionSettings.bAllowJoinViaPresence = false;
		// 	SessionSettings.bAllowJoinViaPresenceFriendsOnly = true;
		// }
		// if(HostSettings.ServerPrivacy == FGameplayTag::RequestGameplayTag("Server.Privacy.InviteOnly"))
		// {
		// 	SessionSettings.bAllowJoinViaPresence = false;
		// 	SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
		// }
		SessionSettings.bAntiCheatProtected = false;
		SessionSettings.bIsDedicated = false;
		SessionSettings.bIsLANMatch = false; //depending on Subsystem
		if(OnlineSubsystem->GetSubsystemName() == TEXT("NULL"))
		{
			SessionSettings.bIsLANMatch = true;
		}
		SessionSettings.bShouldAdvertise = true;
		// SessionSettings.BuildUniqueId todo: Check if this is automatically set
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUsesStats = true;
		SessionSettings.NumPrivateConnections = 0.0f;
		SessionSettings.NumPublicConnections = 2.0f;
		
		SessionSettings.Set<FString>(SESSION_KEY_NAME, HostSettings.ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<FString>(SESSION_KEY_HOST, HostSettings.HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<FString>(SESSION_KEY_PASSWORD, HostSettings.Password, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<FString>(SESSION_KEY_CHARACTER_TYPE, HostSettings.RequiredType.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<FString>(SESSION_KEY_SERVER_MODE, HostSettings.ServerMode.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<int32>(SESSION_KEY_LEVEL_MIN, HostSettings.MinLevel, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<int32>(SESSION_KEY_LEVEL_MAX, HostSettings.MaxLevel, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionSettings.Set<bool>(SESSION_KEY_CHAT_VOICE, HostSettings.bIsVoiceChatEnabled, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		// SessionSettings.Set(SEARCH_KEYWORDS, FString("ProtelumTestInstance"), EOnlineDataAdvertisementType::ViaOnlineService);
		// SessionSettings.Settings.;

		
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
			
	}
}

bool UProtelumGameInstance::IsHostFriend(int32 ServerIndex) const
{
	const FOnlineSessionSearchResult* Result = GetOnlineSessionSearchResultByIndex(ServerIndex);
	if(Result == nullptr)
	{
		return false;
	}
	const FUniqueNetIdPtr HostNetId = Result->Session.OwningUserId;
	FString FriendListName;
	return FriendInterface->IsFriend(0, HostNetId.ToSharedRef().Get(), ToString(EFriendsLists::Default));
}

FString UProtelumGameInstance::GetUsername() const
{
	if(IdentityInterface.IsValid())
	{
		return IdentityInterface->GetPlayerNickname(0);
	}
	return FString();
}

FOnlineSessionSearchResult* UProtelumGameInstance::GetOnlineSessionSearchResultByIndex(int32 SearchResultIndex) const
{
	if(!SessionInterface.IsValid() || !SessionSearch.IsValid() || !SessionSearch->SearchResults.IsValidIndex(SearchResultIndex))
	{
		return nullptr;
	}
	return &SessionSearch->SearchResults[SearchResultIndex];
}
