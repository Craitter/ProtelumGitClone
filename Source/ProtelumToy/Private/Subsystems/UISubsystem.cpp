// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/UISubsystem.h"


#include "AbilitySystem/Interface/ProtelumInteractable.h"
#include "ActorComponents/ContainerComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/UserWidget.h"
#include "Components/MenuAnchor.h"
#include "Kismet/KismetTextLibrary.h"
#include "ProtelumBaseClasses/ProtelumGameInstance.h"
#include "ProtelumBaseClasses/ProtelumPlayerController.h"
#include "Subsystems/SaveGameSubsystem.h"
#include "Widgets/GameOverlay/ContainerGridWidget.h"
#include "Widgets/GameOverlay/ContainerSlotWidget.h"
#include "Widgets/GameOverlay/GameOverlayWidget.h"
#include "Widgets/MainMenu/ServerListItemWidget.h"
#include "Widgets/UtilityWidgets/ContainerDragDropWidget.h"
#include "Widgets/UtilityWidgets/PasswordPromptWidget.h"
#include "Widgets/UtilityWidgets/PromptWidget.h"

UUISubsystem::UUISubsystem()
{
	const ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBP(TEXT("/Game/WidgetSystem/MainMenu/WBP_MainMenu"));
	if(!ensure(MainMenuBP.Class != nullptr)) return;
	MainMenuWidgetClass = MainMenuBP.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> GameOverlayWidgetBP(TEXT("/Game/WidgetSystem/GameOverlay/WBP_GameOverlay"));
	if(!ensure(GameOverlayWidgetBP.Class != nullptr)) return;
	GameOverlayWidgetClass = GameOverlayWidgetBP.Class;

	// const ConstructorHelpers::FClassFinder<UUserWidget> HideoutWidgetBP(TEXT("/Game/WidgetSystem/Utility/WBP_PromptWidget"));
	// if(!ensure(HideoutWidgetBP.Class != nullptr)) return;
	// HideoutWidgetClass = HideoutWidgetBP.Class;
	
	const ConstructorHelpers::FClassFinder<UUserWidget> GenericPromptBP(TEXT("/Game/WidgetSystem/Utility/WBP_PromptWidget"));
	if(!ensure(GenericPromptBP.Class != nullptr)) return;
	GenericPromptClass = GenericPromptBP.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> PasswordPromptBP(TEXT("/Game/WidgetSystem/MainMenu/WBP_ServerPasswordPrompt"));
	if(!ensure(PasswordPromptBP.Class != nullptr)) return;
	PasswordPromptClass = PasswordPromptBP.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> ServerListEntryBP(TEXT("/Game/WidgetSystem/MainMenu/WBP_ServerListEntry"));
	if(!ensure(ServerListEntryBP.Class != nullptr)) return;
	ServerListItemClass = ServerListEntryBP.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> ContainerGridBP(TEXT("/Game/WidgetSystem/Utility/Inventory/WBP_ContainerGrid"));
	if(!ensure(ContainerGridBP.Class != nullptr)) return;
	ContainerGridWidgetClass = ContainerGridBP.Class;
	
	const ConstructorHelpers::FClassFinder<UUserWidget> DragDropBP(TEXT("/Game/WidgetSystem/Utility/Inventory/WBP_DragDropPreview"));
	if(!ensure(DragDropBP.Class != nullptr)) return;
	DragDropWidgetClass = DragDropBP.Class;
	
	PromptHeaderQuitGameText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/menu"), TEXT("menu_quit"));
	PromptQuitGameText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/prompt"), TEXT("prompt_quitgame"));

	PromptHeaderReturnMainMenuText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/menu"), TEXT("menu_return_mainmenu"));
	PromptReturnMainMenuText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/prompt"), TEXT("prompt_mainmenu"));

	PromptHeaderDeleteCharacterText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/character"), TEXT("character_delete"));
	PromptDeleteCharacterText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/prompt"), TEXT("prompt_character_delete"));

	PromptHeaderCharacterCreationFailedText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/character"), TEXT("character_header_create"));
	PromptNoSymbolsText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/prompt"), TEXT("prompt_no_symbols"));
	PromptDuplicatedNameText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/prompt"), TEXT("prompt_duplicated_name"));
	PromptMaxCharacterCountText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/prompt"), TEXT("prompt_max_characters_reached"));
	PromptInvalidNameSizeText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/prompt"), TEXT("prompt_name_size"));

	PromptHeaderNoServerSelectedText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/menu"), TEXT("menu_server_select"));
	PromptNoServerSelectedText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/prompt"), TEXT("prompt_server_no_selection"));

	PromptHeaderInvalidServerText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/menu"), TEXT("menu_server_invalid"));
	PromptInvalidServerText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/prompt"), TEXT("prompt_server_invalid"));

	PromptHeaderCharacterMismatchesServerRequirementsText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/character"), TEXT("character_select"));
	PromptCharacterMismatchesServerRequirementsText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/prompt"), TEXT("prompt_character_mismatch"));
	
	CharacterTypeHumanText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/character"), TEXT("character_type_human"));
	CharacterTypeShamanText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/character"), TEXT("character_type_shaman"));

	ServerModeNormalText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/server"), TEXT("server_mode_normal"));
	ServerModeTutorialText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/server"), TEXT("server_mode_tutorial"));
	
	CharacterTypeDescriptionHumanText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/character"), TEXT("character_description_human"));
	CharacterTypeDescriptionShamanText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/character"), TEXT("character_description_shaman"));	

	PrivacyFriendsOnlyText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/server"), TEXT("server_privacy_friends"));
	PrivacyInviteText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/server"), TEXT("server_privacy_invite"));
	PrivacyPublicText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/server"), TEXT("server_privacy_public"));
	PrivacyNoPasswordFilterText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/server"), TEXT("server_privacy_no_password"));
		
	JoinText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/menu"), TEXT("menu_join"));
	ShowAllFilterText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/server"), TEXT("server_show_all"));
	LevelRangeText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/server"), TEXT("server_level_range"));
	DefaultServerName = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/server"), TEXT("server_name_default"));

	InteractOpenHoldText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/interact"), TEXT("interact_open_hold"));
	InteractOpenPressText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/interact"), TEXT("interact_open_press"));
	InteractPickupHoldText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/interact"), TEXT("interact_pickup_hold"));
	InteractPickupPressText = FText::FromStringTable(TEXT("/Game/WidgetSystem/StringTables/interact"), TEXT("interact_pickup_press"));

	ShowAllTag = FGameplayTag::RequestGameplayTag("Server.ShowAll");
	HumanTag = FGameplayTag::RequestGameplayTag("Character.Human");
	ShamanTag = FGameplayTag::RequestGameplayTag("Character.Shaman");
	ServerModeTutorialTag = FGameplayTag::RequestGameplayTag("Server.Mode.Tutorial");
	ServerModeNormalTag = FGameplayTag::RequestGameplayTag("Server.Mode.Normal");
	PrivacyInviteTag = FGameplayTag::RequestGameplayTag("Server.Privacy.InviteOnly");
	PrivacyPublicTag = FGameplayTag::RequestGameplayTag("Server.Privacy.Public");
	PrivacyFriendsOnlyTag = FGameplayTag::RequestGameplayTag("Server.Privacy.FriendsOnly");
	PrivacyNoPasswordTag = FGameplayTag::RequestGameplayTag("Server.Privacy.NoPassword");


	
}

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if(GameInstance.IsValid())
	{
		SaveGameSubsystem = GameInstance->GetSubsystem<USaveGameSubsystem>();
		ProtelumGameInstance = Cast<UProtelumGameInstance>(GameInstance);
	}
	if(ProtelumGameInstance.IsValid())
	{
		FOnSearchResultsReadyDelegate* OnSearchResultsReadyDelegate = ProtelumGameInstance->GetOnSearchResultsReadyDelegate();
		if(OnSearchResultsReadyDelegate != nullptr)
		{
			OnSearchResultsReadyDelegate->AddUObject(this, &ThisClass::OnSearchResultsReady);
		}
	}
	OnServerListReadyDelegate.AddUObject(this, &ThisClass::UpdateShownServerList);
}

void UUISubsystem::LoadInterfaceBaseWidget(EBaseWidgetType WidgetType)
{
	BaseWidgetType = WidgetType;
	
	TSubclassOf<UProtelumUserInterfaceBaseWidget> TempWidgetClass = {nullptr};
	switch (BaseWidgetType)
	{
		case EBaseWidgetType::MainMenu:
			TempWidgetClass = MainMenuWidgetClass;
			break;
		case EBaseWidgetType::MissionOverlay:
			TempWidgetClass = GameOverlayWidgetClass;
			break;
		case EBaseWidgetType::HideoutOverlay:
			TempWidgetClass = HideoutWidgetClass;
			break;
		default: ;
	}
	UserInterfaceBase = CreateWidget<UProtelumUserInterfaceBaseWidget>(GetGameInstance(), TempWidgetClass);
	if(UserInterfaceBase == nullptr)
	{
		return;
	}
	//Todo: Change...
	UserInterfaceBase->AddToViewport();
	// switch (UserInterfaceBase->GetWidgetBaseType())
	// {
	// 	case EBaseWidgetType::MainMenu:
	// 		SetMenuInput(UserInterfaceBase);
	// 		break;
	// 	case EBaseWidgetType::MissionOverlay:
	// 		GameOverlayWidget = Cast<UGameOverlayWidget>(UserInterfaceBase);
	// 		SetGameOnly();
	// 		break;
	// 	case EBaseWidgetType::HideoutOverlay:
	// 		SetGameOnly();
	// 		break;
	// 	default: ;
	// }
	
}

FOnPlayerPromptChoice* UUISubsystem::CreatePrompt(EPromptType PromptType, EPromptMessageType ErrorMessageType,
                                                  bool& WasSuccessful) const
{
	if(UserInterfaceBase != nullptr)
	{
		const TObjectPtr<UPromptWidget> Prompt = CreateWidget<UPromptWidget>(GetGameInstance(), GenericPromptClass);
		if(Prompt != nullptr)
		{
			FText PromptHeader;
			const FText PromptMessage = CreatePromptMessage(ErrorMessageType, PromptHeader);
			EnablePrompt(Prompt);
			Prompt->SetPromptType(PromptType);
			Prompt->SetPromptText(PromptMessage, PromptHeader);
			FOnPlayerPromptChoice* Choice = Prompt->GetPromptReplyDelegate();
			if(Choice != nullptr)
			{
				Choice->AddUObject(this, &UUISubsystem::OnPromptChoiceMade);
			}
			WasSuccessful = true;
			return Choice;
		}
	}
	// if()
	WasSuccessful = false;
	return nullptr;
	
}

FOnPasswordEnteringConfirmedDelegate* UUISubsystem::CreatePasswordPrompt() const
{
	if(UserInterfaceBase != nullptr)
	{
		const TObjectPtr<UPasswordPromptWidget> Prompt = CreateWidget<UPasswordPromptWidget>(GetGameInstance(), PasswordPromptClass);
		if(Prompt != nullptr)
		{
			EnablePrompt(Prompt);
			FOnPasswordEnteringConfirmedDelegate* Choice = Prompt->GetOnPasswordEnteringConfirmedDelegate();
			// WasSuccessful = true;
			return Choice;
		}
	}
	// WasSuccessful = false;
	return nullptr;
}

void UUISubsystem::EnablePrompt(TWeakObjectPtr<UUserWidget> PromptToFocus) const
{
	if(UserInterfaceBase != nullptr)
	{
		UserInterfaceBase->DisableSwitch();
	}
	const TWeakObjectPtr<UMenuAnchor> MenuAnchor = UserInterfaceBase->GetPromptSlot();
	if(MenuAnchor.IsValid())
	{
		MenuAnchor->SetContent(PromptToFocus.Get());
		MenuAnchor->Open(true);
	}
}

void UUISubsystem::DisablePrompt() const
{
	if(UserInterfaceBase != nullptr)
	{
		UserInterfaceBase->EnableSwitch();
	}
	const TWeakObjectPtr<UMenuAnchor> MenuAnchor = UserInterfaceBase->GetPromptSlot();
	if(MenuAnchor.IsValid())
	{
		const TWeakObjectPtr<UWidget> WidgetToRemove = MenuAnchor->GetContent();
		if(WidgetToRemove != nullptr)
		{
			WidgetToRemove->RemoveFromParent();
		}
		MenuAnchor->Close();
	}
}

FText UUISubsystem::CreatePromptMessage(EPromptMessageType ErrorMessageType, FText& PromptHeader) const
{
	if(UserInterfaceBase != nullptr)
	{
		switch (ErrorMessageType)
		{
		case EPromptMessageType::QuitGame:
			PromptHeader = PromptHeaderQuitGameText;
			return PromptQuitGameText;
		case EPromptMessageType::MainMenu:
			PromptHeader = PromptHeaderReturnMainMenuText;
			return PromptReturnMainMenuText;
		case EPromptMessageType::DeleteCharacter:
			PromptHeader = PromptHeaderDeleteCharacterText;
			return PromptDeleteCharacterText;
		case EPromptMessageType::NoSymbols:
			PromptHeader = PromptHeaderCharacterCreationFailedText;
			return PromptNoSymbolsText;
		case EPromptMessageType::DuplicatedName:
			PromptHeader = PromptHeaderCharacterCreationFailedText;
			return PromptDuplicatedNameText;
		case EPromptMessageType::MaxCharacterCount:
			PromptHeader = PromptHeaderCharacterCreationFailedText;
			return PromptMaxCharacterCountText;
		case EPromptMessageType::InvalidNameSize:
			PromptHeader = PromptHeaderCharacterCreationFailedText;
			return PromptInvalidNameSizeText;
		case EPromptMessageType::NoServerSelected:
			PromptHeader = PromptHeaderNoServerSelectedText;
			return PromptNoServerSelectedText;
		case EPromptMessageType::ServerIsNotValid:
			PromptHeader = PromptHeaderInvalidServerText;
			return PromptInvalidServerText;
		case EPromptMessageType::CharacterNotMatchingRequirements:
			PromptHeader = PromptHeaderCharacterMismatchesServerRequirementsText;
			return PromptCharacterMismatchesServerRequirementsText;
		default: ;
		}
	}
	return FText::GetEmpty();
}

void UUISubsystem::OnPromptChoiceMade(EPromptReply Reply) const
{
	DisablePrompt();
}

void UUISubsystem::HostGame(const FText& InServerName, const FText& InPassword, const FText& InServerVisibility,
	const FText& InServerMode, const int32 InMinLevel, const int32 InMaxLevel, const bool bInVoiceChat) const
{
	const FGameplayTag ServerPrivacy = GetServerPrivacyAsTagFromText(InServerVisibility);
	
	const FGameplayTag ServerMode = GetServerModeAsTagFromText(InServerMode);
	
	FGameplayTag CharacterType = FGameplayTag::EmptyTag;
	if(SaveGameSubsystem.IsValid())
	{
		CharacterType = SaveGameSubsystem->GetCharacterType();
		
	}
	if(CharacterType == HumanTag)
	{
		CharacterType = ShamanTag;
	}
	else
	{
		CharacterType = HumanTag;
	}
	
	FHostSettings HostSettings;
	HostSettings.ServerName = InServerName.ToString();
	HostSettings.HostName = FString();
	if(ProtelumGameInstance.IsValid())
	{
		HostSettings.HostName = ProtelumGameInstance->GetUsername();
	}
	HostSettings.Password = InPassword.ToString();
	HostSettings.ServerPrivacy = ServerPrivacy;
	HostSettings.RequiredType = CharacterType;
	HostSettings.ServerMode = ServerMode;
	HostSettings.MinLevel = InMinLevel;
	HostSettings.MaxLevel = InMaxLevel;
	HostSettings.bIsVoiceChatEnabled = bInVoiceChat;
	HostSettings.Index = 0;
	if(ProtelumGameInstance.IsValid())
	{
		ProtelumGameInstance->HostGame(HostSettings);
	}
}

void UUISubsystem::RefreshServerList()
{
	if(ProtelumGameInstance.IsValid() && !bIsServerListRefreshing)
	{
		bIsServerListRefreshing = true;
		ProtelumGameInstance->FindSessions();
	}
}



void UUISubsystem::JoinGame(int32 Index) const
{
	if(ProtelumGameInstance.IsValid())
	{
		ProtelumGameInstance->Join(Index);
	}
}

bool UUISubsystem::IsServerStillValid(int32 Index) const
{
	if(ProtelumGameInstance.IsValid() && ProtelumGameInstance->IsServerStillValid(Index))
	{
		return true;
	}
	return false;
}

void UUISubsystem::QuitGame() const
{
	const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if(GameInstance.IsValid())
	{
		const TWeakObjectPtr<APlayerController> PlayerController = GameInstance->GetFirstLocalPlayerController();
		if(PlayerController.IsValid())
		{
			PlayerController->ConsoleCommand("quit");
			return;
		}
	}
	UE_LOG(LogTemp, Error , TEXT("%s() Game could not be closed"), *FString(__FUNCTION__));
}

FString UUISubsystem::GetUsername() const
{
	if(ProtelumGameInstance.IsValid())
	{
		return ProtelumGameInstance->GetUsername();
	}
	return FString();
}

FText UUISubsystem::GetDefaultServerName() const
{
	FFormatNamedArguments Args; //todo: change to localizable
	Args.Add(TEXT("username"), FText::FromString(GetUsername()));
	return FText::Format(DefaultServerName, Args);
}

FText UUISubsystem::GetPreviousCharacterType(const FText& CurrentType, const bool bCanBeAll) const
{
	const FGameplayTag CurrentTypeTag = GetCharacterTypeAsTagFromText(CurrentType);
	if(bCanBeAll && CurrentTypeTag == ShowAllTag)
	{
		return GetCharacterTypeAsTextFromTag(HumanTag);
	}
	if(CurrentTypeTag == HumanTag)
	{
		return GetCharacterTypeAsTextFromTag(ShamanTag);
	}
	if(CurrentTypeTag == ShamanTag)
	{
		if(bCanBeAll)
		{
			return GetCharacterTypeAsTextFromTag(ShowAllTag);
		}
		return GetCharacterTypeAsTextFromTag(HumanTag);
	}
	if(bCanBeAll)
	{
		return GetCharacterTypeAsTextFromTag(ShowAllTag);
	}
	return GetCharacterTypeAsTextFromTag(ShamanTag);
}

FText UUISubsystem::GetNextCharacterType(const FText& CurrentType, const bool bCanBeAll) const
{
	const FGameplayTag CurrentTypeTag = GetCharacterTypeAsTagFromText(CurrentType);
	if(bCanBeAll && CurrentTypeTag == ShowAllTag)
	{
		return GetCharacterTypeAsTextFromTag(ShamanTag);
	}
	if(CurrentTypeTag == ShamanTag)
	{
		return GetCharacterTypeAsTextFromTag(HumanTag);
	}
	if(CurrentTypeTag == HumanTag)
	{
		if(bCanBeAll)
		{
			return GetCharacterTypeAsTextFromTag(ShowAllTag);
		}
		return GetCharacterTypeAsTextFromTag(ShamanTag);
	}
	if(bCanBeAll)
	{
		return GetCharacterTypeAsTextFromTag(ShowAllTag);
	}
	return GetCharacterTypeAsTextFromTag(ShamanTag);
}

FText UUISubsystem::CreateCharacter(const FText& CharacterName, const FText& CharacterType, bool& bWasSuccessful) const
{
	bWasSuccessful = false;
	if(!SaveGameSubsystem.IsValid())
	{
		return FText::GetEmpty();
	}
	EPromptMessageType ErrorType;
	const FGameplayTag TypeTag = GetCharacterTypeAsTagFromText(CharacterType);
	if(SaveGameSubsystem->CreateNewSaveGame(CharacterName.ToString(), TypeTag, ErrorType))
	{
		bWasSuccessful = true;
		return CharacterName;
	}
	bool bSuccess;
	CreatePrompt(EPromptType::OkPrompt, ErrorType, bSuccess);
	return FText::GetEmpty();	
}

FText UUISubsystem::DeleteCharacter(bool& bWasLastCharacter) const
{
	if(SaveGameSubsystem.IsValid())
	{
		FString SaveGameToDelete;
		FString NewCharacterName;
		SaveGameSubsystem->GetCharacterName(SaveGameToDelete);
		SaveGameSubsystem->LoadNextSaveGame();
		SaveGameSubsystem->GetCharacterName(NewCharacterName);
		if(SaveGameToDelete == NewCharacterName)
		{
			bWasLastCharacter = true;
		}
		SaveGameSubsystem->DeleteSaveGame(SaveGameToDelete);
		return FText::FromString(NewCharacterName);
	}
	return FText::GetEmpty();
}

FText UUISubsystem::GetActiveCharacter() const
{
	if(SaveGameSubsystem.IsValid())
	{
		if(SaveGameSubsystem->IsActiveSaveGameLoaded())
		{
			FString CharacterName;
			SaveGameSubsystem->GetCharacterName(CharacterName);
			return FText::FromString(CharacterName);
		}
	}
	return FText::GetEmpty();
}

FText UUISubsystem::GetNextCharacter() const
{
	if(SaveGameSubsystem.IsValid())
	{
		SaveGameSubsystem->LoadNextSaveGame();
		FString CharacterName;
		SaveGameSubsystem->GetCharacterName(CharacterName);
		return FText::FromString(CharacterName);
	}
	
	return FText::GetEmpty();
}

FText UUISubsystem::GetPreviousCharacter() const
{
	if(SaveGameSubsystem.IsValid())
	{
		SaveGameSubsystem->LoadPreviousSaveGame();
		FString CharacterName;
		SaveGameSubsystem->GetCharacterName(CharacterName);
		return FText::FromString(CharacterName);
	}
	return FText::GetEmpty();
}

FText UUISubsystem::GetCharacterTypeDescription(const FText& CurrentType) const
{
	if(CurrentType.EqualTo(CharacterTypeHumanText))
	{
		return CharacterTypeDescriptionHumanText;
	}
	return CharacterTypeDescriptionShamanText;
}

FText UUISubsystem::GetCharacterType() const
{
	if(SaveGameSubsystem.IsValid())
	{
		const FGameplayTag Type = SaveGameSubsystem->GetCharacterType();
		return GetCharacterTypeAsTextFromTag(Type);
	}
	return FText::GetEmpty();
}

FText UUISubsystem::GetCharacterLevel() const
{
	if(SaveGameSubsystem.IsValid())
	{
		return UKismetTextLibrary::Conv_IntToText(SaveGameSubsystem->GetCharacterLevel());
	}
	return FText::GetEmpty();
}

FText UUISubsystem::GetCharacterCraftedArtifactsCount() const
{
	if(SaveGameSubsystem.IsValid())
	{
		const FGameplayTagContainer* CraftedArtifacts = SaveGameSubsystem->GetCraftedArtifacts();
		if(CraftedArtifacts != nullptr)
		{
			return UKismetTextLibrary::Conv_IntToText(CraftedArtifacts->Num());
		}
	}
	return FText::GetEmpty();
}

FText UUISubsystem::GetCharacterLastPlayedWith() const
{
	if(SaveGameSubsystem.IsValid())
	{
		return FText::FromString(SaveGameSubsystem->GetLastPlayedPlayerName());
	}
	return FText::GetEmpty();
}

FText UUISubsystem::GetCharacterPlayTimeInHours() const
{
	if(SaveGameSubsystem.IsValid())
	{
		const float TimePlayed = SaveGameSubsystem->GetCharacterTimePlayed();
		if(TimePlayed + SMALL_NUMBER >= 0.0f)
		{
			return UKismetTextLibrary::Conv_DoubleToText(TimePlayed, FromZero);
		}
	}
	return FText::GetEmpty();
}



void UUISubsystem::ShowInteractPrompt(float TimeToHold, FInteractMessageInformation InteractMessageInformation) const
{
	if(GameOverlayWidget == nullptr)
	{
		return;
	}
	bool bIsHoldingInteraction = false;
	if(TimeToHold > 0.0f)
	{
		bIsHoldingInteraction = true;
	}
	
	FText OutDisplayText;
	GetInteractionText(InteractMessageInformation.InteractMessageType, bIsHoldingInteraction, OutDisplayText);
	//Todo: GetKey mit marvi reden!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//Todo: UseName to make full text
	//Hold {key} to Open {noun}
	if(OutDisplayText.IsEmpty())
	{
		return;
	}
	GameOverlayWidget->ShowInteractionPrompt(OutDisplayText, TimeToHold);
}

void UUISubsystem::HideInteractPrompt() const
{
	if(GameOverlayWidget == nullptr)
	{
		return;
	}
	GameOverlayWidget->HideInteractionPrompt();
}

void UUISubsystem::StartInteractionTimer(float TimeToHold) const
{
	if(GameOverlayWidget == nullptr)
	{
		return;
	}
	GameOverlayWidget->StartInteractionTimer(TimeToHold);
}

void UUISubsystem::StopInteractionTimer() const
{
	if(GameOverlayWidget == nullptr)
	{
		return;
	}
	GameOverlayWidget->StopInteractionTimer();
}

FText UUISubsystem::GetPreviousServerMode(const FText& CurrentServerMode, const bool bCanBeAll) const
{
	const FGameplayTag CurrentServerModeTag = GetServerModeAsTagFromText(CurrentServerMode);
	if(bCanBeAll && CurrentServerModeTag == ShowAllTag)
	{
		return GetServerModeAsTextFromTag(ServerModeNormalTag);
	}
	if(CurrentServerModeTag == ServerModeTutorialTag)
	{
		return GetServerModeAsTextFromTag(ServerModeNormalTag);
	}
	if(CurrentServerModeTag == ServerModeNormalTag)
	{
		if(bCanBeAll)
		{
			return GetServerModeAsTextFromTag(ShowAllTag);
		}
		return GetServerModeAsTextFromTag(ServerModeTutorialTag);
	}
	if(bCanBeAll)
	{
		return GetServerModeAsTextFromTag(ShowAllTag);
	}
	return GetServerModeAsTextFromTag(ServerModeNormalTag);
	
}

FText UUISubsystem::GetNextServerMode(const FText& CurrentServerMode, bool bCanBeAll) const
{
	const FGameplayTag CurrentServerModeTag = GetServerModeAsTagFromText(CurrentServerMode);
	
	if(bCanBeAll && CurrentServerModeTag == ShowAllTag)
	{
		return GetServerModeAsTextFromTag(ServerModeNormalTag);
	}
	if(CurrentServerModeTag == ServerModeTutorialTag)
	{
		if(bCanBeAll)
		{
			return GetServerModeAsTextFromTag(ShowAllTag);
		}
		return GetServerModeAsTextFromTag(ServerModeNormalTag);
	}
	if(CurrentServerModeTag == ServerModeNormalTag)
	{
		return GetServerModeAsTextFromTag(ServerModeTutorialTag);
	}
	if(bCanBeAll)
	{
		return GetServerModeAsTextFromTag(ShowAllTag);
	}
	return GetServerModeAsTextFromTag(ServerModeNormalTag);
}

FText UUISubsystem::GetPreviousServerPrivacy(const FText& CurrentLobbyVisibility, const bool bIsHostPrivacy) const
{
	const FGameplayTag CurrentLobbyVisibilityTag = GetServerPrivacyAsTagFromText(CurrentLobbyVisibility);
	if(CurrentLobbyVisibilityTag == PrivacyInviteTag || CurrentLobbyVisibilityTag == PrivacyNoPasswordTag)
	{
		return GetServerPrivacyAsTextFromTag(PrivacyFriendsOnlyTag);
	}
	if(CurrentLobbyVisibilityTag == PrivacyPublicTag)
	{
		if(bIsHostPrivacy)
		{
			return GetServerPrivacyAsTextFromTag(PrivacyInviteTag);
		}
		return GetServerPrivacyAsTextFromTag(PrivacyNoPasswordTag);
	}
	return GetServerPrivacyAsTextFromTag(PrivacyPublicTag);
}

FText UUISubsystem::GetNextServerPrivacy(const FText& CurrentServerPrivacy, bool bIsHostPrivacy) const
{
	const FGameplayTag CurrentLobbyVisibilityTag = GetServerPrivacyAsTagFromText(CurrentServerPrivacy);
	if(CurrentLobbyVisibilityTag == PrivacyPublicTag)
	{
		return GetServerPrivacyAsTextFromTag(PrivacyFriendsOnlyTag);
	}
	if(CurrentLobbyVisibilityTag == PrivacyFriendsOnlyTag)
	{
		if(bIsHostPrivacy)
		{
			return GetServerPrivacyAsTextFromTag(PrivacyInviteTag);
		}
		return GetServerPrivacyAsTextFromTag(PrivacyNoPasswordTag);
	}
	return GetServerPrivacyAsTextFromTag(PrivacyPublicTag);
}

const FText& UUISubsystem::GetJoinText()
{
	return JoinText;
}

bool UUISubsystem::GetIsServerListRefreshing() const
{
	return bIsServerListRefreshing;
}

FOnServerListEntryCreatedDelegate* UUISubsystem::GetOnServerListItemCreatedDelegate()
{
	return &OnServerListEntryCreatedDelegate;
}

FOnServerListReadyDelegate* UUISubsystem::GetOnServerListReadyDelegate()
{
	return &OnServerListReadyDelegate;
}

void UUISubsystem::OnSearchResultsReady(const TArray<FHostSettings>& InFoundSessionsHostData)
{
	FoundSessionsHostData = InFoundSessionsHostData;
	bIsServerListRefreshing = false;
	OnServerListReadyDelegate.Broadcast();
}

void UUISubsystem::SetServerSearchIsResultFiltered(bool bShouldFilter)
{
	ActiveSearchSettings.bShouldBeFiltered = bShouldFilter;
	if(!bIsServerListRefreshing)
	{
		UpdateShownServerList();
	}
}

void UUISubsystem::SetServerSearchPrivacyFilter(const FText& SearchPrivacy)
{
	const FGameplayTag SearchServerPrivacy = GetServerPrivacyAsTagFromText(SearchPrivacy);
	
	ActiveSearchSettings.SearchPrivacyFilter = SearchServerPrivacy;
	if(!bIsServerListRefreshing)
	{
		UpdateShownServerList();
	}
}

void UUISubsystem::SetServerSearchRequiredCharacter(const FText& RequiredCharacter)
{
	if(RequiredCharacter.EqualTo(CharacterTypeHumanText))
	{
		ActiveSearchSettings.RequiredTypeFilter = HumanTag;
	}
	else if(RequiredCharacter.EqualTo(CharacterTypeShamanText))
	{
		ActiveSearchSettings.RequiredTypeFilter = ShamanTag;
	}
	else
	{
		ActiveSearchSettings.RequiredTypeFilter = FGameplayTag::EmptyTag;
	}
	if(!bIsServerListRefreshing)
	{
		UpdateShownServerList();
	}
}

void UUISubsystem::SetServerSearchServerModeFilter(const FText& ServerModeFilter)
{
	if(ServerModeFilter.EqualTo(ServerModeTutorialText))
	{
		ActiveSearchSettings.ServerModeFilter = ServerModeTutorialTag;
	}
	else if(ServerModeFilter.EqualTo(ShowAllFilterText))
	{
		ActiveSearchSettings.ServerModeFilter = FGameplayTag::EmptyTag;
	}
	else
	{
		ActiveSearchSettings.ServerModeFilter = ServerModeNormalTag;
	}
	if(!bIsServerListRefreshing)
	{
		UpdateShownServerList();
	}
}

void UUISubsystem::SetServerSearchAllowedLevelFilter(const uint32 InAllowedLevel)
{
	ActiveSearchSettings.AllowedLevelFilter = InAllowedLevel;
	if(!bIsServerListRefreshing)
	{
		UpdateShownServerList();
	}
}

void UUISubsystem::SetServerSearchVoiceChatFilter(bool bIsVoiceChatAllowed)
{
	ActiveSearchSettings.bIsVoiceChatEnabledFilter = bIsVoiceChatAllowed;
	if(!bIsServerListRefreshing)
	{
		UpdateShownServerList();
	}
}

FOnServerListFilterChanged* UUISubsystem::GetOnServerListFilterChanged()
{
	return &OnServerListFilterChanged;
}

bool UUISubsystem::IsServerPasswordProtected(int32 ServerListIndex) const
{
	if(!ProtelumGameInstance.IsValid())
	{
		return false;
	}
	if(ProtelumGameInstance->IsServerPasswordProtected(ServerListIndex))
	{
		return true;
	}
	return false;
}

bool UUISubsystem::IsPasswordCorrect(int32 ServerListIndex, const FString& ServerPassword) const
{
	if(ProtelumGameInstance.IsValid())
	{
		return ProtelumGameInstance->IsServerPasswordCorrect(ServerListIndex, ServerPassword);
	}
	return false;
}

bool UUISubsystem::IsCharacterValidForSelectedServer(int32 ServerListIndex) const
{
	if(!SaveGameSubsystem.IsValid() || !ProtelumGameInstance.IsValid())
	{
		return false;
	}
	FGameplayTag ServerRequestedCharacterType;
	int32 ServerMinLevel;
	int32 ServerMaxLevel;
	ProtelumGameInstance->GetServerJoinRestrictions(ServerListIndex ,ServerMinLevel, ServerMaxLevel, ServerRequestedCharacterType);

	const FGameplayTag ClientCharacterType = SaveGameSubsystem->GetCharacterType();
	const int32 ClientCharacterLevel = SaveGameSubsystem->GetCharacterLevel();
	if(ClientCharacterType != ServerRequestedCharacterType || ClientCharacterLevel > ServerMaxLevel || ClientCharacterLevel < ServerMinLevel)
	{
		return false;
	}
	return true;
}

TWeakObjectPtr<UTexture2D> UUISubsystem::GetIsPasswordLockedImage(const FString& Password)
{
	return nullptr;
}

TWeakObjectPtr<UTexture2D> UUISubsystem::GetIsVoiceChatEnabledImage(bool bIsVoiceChatEnabled)
{
	return nullptr;
}

FText UUISubsystem::GetLevelRangeAsText(uint32 MinLevel, uint32 MaxLevel) const
{
	
	FFormatNamedArguments Args; //todo: change to localizable
	Args.Add(TEXT("min_level"), MinLevel);
	Args.Add(TEXT("max_level"), MaxLevel);
	return FText::Format(LevelRangeText, Args);
}

bool UUISubsystem::MeetsPrivacyFilter(const FHostSettings& HostData) const
{
	if(ActiveSearchSettings.SearchPrivacyFilter == PrivacyFriendsOnlyTag)
	{
		if(ProtelumGameInstance.IsValid())
		{
			if(!ProtelumGameInstance->IsHostFriend(HostData.Index))
			{
				return false;
			}
		}
	}

	else if(ActiveSearchSettings.SearchPrivacyFilter == PrivacyNoPasswordTag)
	{
		if(!HostData.Password.Equals(FString()))
		{
			return false;
		}
	}
	
	//case PrivacyPublicTag: ;
	return true;
}

bool UUISubsystem::MeetsCharacterTypeFilter(const FHostSettings& HostData) const
{
	if(ActiveSearchSettings.RequiredTypeFilter == FGameplayTag::EmptyTag || HostData.RequiredType == ActiveSearchSettings.RequiredTypeFilter)
	{
		return true;
	}
	return false;
}

bool UUISubsystem::MeetsServerModeFilter(const FHostSettings& HostData) const
{
	if(ActiveSearchSettings.ServerModeFilter == FGameplayTag::EmptyTag || ActiveSearchSettings.ServerModeFilter == HostData.ServerMode)
	{
		return true;
	}
	return false;
}

bool UUISubsystem::MeetsAllowedLevelFilter(const FHostSettings& HostData) const
{
	const int32 AllowedLevel = ActiveSearchSettings.AllowedLevelFilter;
	if(AllowedLevel <= HostData.MaxLevel && AllowedLevel >= HostData.MinLevel)
	{
		return true;
	}
	return false;
}

bool UUISubsystem::MeetsVoiceChatFilter(const FHostSettings& HostData) const
{
	if(ActiveSearchSettings.bIsVoiceChatEnabledFilter == HostData.bIsVoiceChatEnabled)
	{
		return true;
	}
	return false;
}

void UUISubsystem::UpdateShownServerList()
{
	OnServerListFilterChanged.Broadcast();
	for(const auto& HostData : FoundSessionsHostData)
	{
	
		if(ActiveSearchSettings.bShouldBeFiltered)
		{
			if(!MeetsPrivacyFilter(HostData))
			{
				continue;
			}
			if(!MeetsCharacterTypeFilter(HostData))
			{
				continue;
			}
			if(!MeetsServerModeFilter(HostData))
			{
				continue;
			}
			if(!MeetsAllowedLevelFilter(HostData))
			{
				continue;
			}
			if(!MeetsVoiceChatFilter(HostData))
			{
				continue;
			}
		}
		FServerListEntryData ListItemData;
		ListItemData.ServerName = FText::FromString(HostData.ServerName);
		ListItemData.HostName = FText::FromString(HostData.HostName);
		ListItemData.RequiredType = GetCharacterTypeAsTextFromTag(HostData.RequiredType);
		ListItemData.ServerMode = GetServerModeAsTextFromTag(HostData.ServerMode);

		ListItemData.LevelRange = GetLevelRangeAsText(HostData.MinLevel, HostData.MaxLevel);
		ListItemData.PasswordLockedImage = GetIsPasswordLockedImage(HostData.Password);
		ListItemData.VoiceChatImage = GetIsVoiceChatEnabledImage(HostData.bIsVoiceChatEnabled);
		ListItemData.ServerIndex = HostData.Index;
		CreateServerListEntry(ListItemData);
	}

}

void UUISubsystem::CreateServerListEntry(const FServerListEntryData& ListEntryData) const
{
	const TObjectPtr<UServerListItemWidget> ServerListItem = CreateWidget<UServerListItemWidget>(GetGameInstance(), ServerListItemClass);
	ServerListItem->SetServerListItem(ListEntryData);
	OnServerListEntryCreatedDelegate.Broadcast(ServerListItem);
}

bool UUISubsystem::OpenInventory(UContainerComponent* InParentContainer) const
{
	if(GameOverlayWidget != nullptr)
	{
		const TObjectPtr<UContainerGridWidget> ContainerGrid = CreateWidget<UContainerGridWidget>(GetGameInstance(), ContainerGridWidgetClass);
		if(ContainerGrid == nullptr)
		{
			return false;
		}
		ContainerGrid->ShowContainerContent(InParentContainer);
		GameOverlayWidget->ShowInventory(ContainerGrid);
		SetMenuInput(GameOverlayWidget);
		return true;
	}
	return false;
}

void UUISubsystem::OpenInventoryExpanded()
{
}

void UUISubsystem::ContainerSlotClicked(TWeakObjectPtr<UContainerSlotWidget> InContainerSlotWidget, FKey ImpactKey)
{
	
	if(InContainerSlotWidget == nullptr || GameOverlayWidget == nullptr)
	{
		return;
	}
	const TWeakObjectPtr<UContainerComponent> InParentContainer = InContainerSlotWidget->GetParentContainer();
	const FContainerSlot* InContainerSlot = InContainerSlotWidget->GetSlotContent();
	if(!InParentContainer.IsValid() || InContainerSlot == nullptr)
	{
		return;
	}
	if(CachedSlot != nullptr && CachedSlot->GetParentContainer() != nullptr)
	{
		const TWeakObjectPtr<UContainerComponent> CachedParentContainer = CachedSlot->GetParentContainer();
		if(CachedParentContainer.IsValid())
		{
			bool bDragSwappedItem = false;
			const TWeakObjectPtr<UContainerComponent> SourceContainer = CachedParentContainer;
			const int32 SourceIndex = CachedSlot->GetReferencedSlotIndex();
			const TWeakObjectPtr<UContainerComponent> DestinationContainer = InParentContainer;
			const int32 DestinationIndex = InContainerSlotWidget->GetReferencedSlotIndex();
			DestinationContainer->SwapSlots(SourceContainer, SourceIndex, DestinationIndex, bDragSwappedItem);
			GameOverlayWidget->SetDragWidget(nullptr);
			if(!bDragSwappedItem)
			{
				CachedSlot->SetVisibility(ESlateVisibility::Visible);
				CachedSlot = nullptr;
				return;
			}
		}
		
	}
	else
	{
		CachedSlot = InContainerSlotWidget;
	}
	if(CachedSlot == nullptr)
	{
		return;
	}
	CachedSlot->SetVisibility(ESlateVisibility::Hidden);
	//Todo: Determine Button
	const FCollectableItem* CollectableItem = nullptr;
	const TWeakObjectPtr<UContainerComponent> CachedParentContainer = CachedSlot->GetParentContainer();
	const FContainerSlot* CachedContainerSlot = CachedSlot->GetSlotContent();
	if(CachedParentContainer.IsValid())
	{
		CollectableItem = CachedParentContainer->GetItemReferenceByTag(CachedContainerSlot->ItemTag);
	}
	const TObjectPtr<UContainerDragDropWidget> DragDropWidget = CreateWidget<UContainerDragDropWidget>(GameOverlayWidget, DragDropWidgetClass);
	if(DragDropWidget != nullptr && CollectableItem != nullptr && CollectableItem->Thumbnail != nullptr)
	{
		DragDropWidget->SetDragDropImage(CollectableItem->Thumbnail);
		DragDropWidget->SetDesiredSizeInViewport(DragDropWidget->GetDesiredSize());
		DragDropWidget->AddToViewport();
		GameOverlayWidget->SetDragWidget(DragDropWidget);
	}
}


void UUISubsystem::InitMenuInput()
{
	SetMenuInput(UserInterfaceBase);
}

void UUISubsystem::InitHideoutInput()
{
	SetGameOnly();
}

void UUISubsystem::InitMissionInput()
{
	GameOverlayWidget = Cast<UGameOverlayWidget>(UserInterfaceBase);
	SetGameOnly();
}

FVector2D UUISubsystem::GetGameResolution()
{
	FVector2D Result;

	Result.X = GSystemResolution.ResX;
	Result.Y = GSystemResolution.ResY;

	return Result;
}

FVector2D UUISubsystem::GetViewportSize()
{
	FVector2D Result = FVector2D( 1, 1 );

	if ( GEngine && GEngine->GameViewport )
	{
		GEngine->GameViewport->GetViewportSize( /*out*/Result );
	}

	return Result;
}

void UUISubsystem::SetGameOnly() const
{
	const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if(GameInstance.IsValid())
	{
		const TWeakObjectPtr<APlayerController> PlayerController = GameInstance->GetPrimaryPlayerController();
		if(PlayerController.IsValid() && PlayerController->IsLocalController())
		{
			const TWeakObjectPtr<AProtelumPlayerController> ProtelumPlayerController = Cast<AProtelumPlayerController>(PlayerController);
			if(ProtelumPlayerController.IsValid())
			{
				ProtelumPlayerController->ActivateGameInput();
			}
		}
	}	
}

void UUISubsystem::SetMenuInput(const TWeakObjectPtr<UUserWidget> NewWidget) const
	{
	const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if(GameInstance.IsValid())
	{
		const TWeakObjectPtr<APlayerController> PlayerController = GameInstance->GetPrimaryPlayerController();
		if(PlayerController.IsValid())
		{
			const TWeakObjectPtr<AProtelumPlayerController> ProtelumPlayerController = Cast<AProtelumPlayerController>(PlayerController);
			if(ProtelumPlayerController.IsValid())
			{
				FInputModeGameAndUI Mode;
				Mode.SetWidgetToFocus(NewWidget->TakeWidget());
				Mode.SetHideCursorDuringCapture(false);
				Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
				ProtelumPlayerController->ActivateMenuInput(Mode);
			}
		}
	}	
}

FGameplayTag UUISubsystem::GetCharacterTypeAsTagFromText(const FText& CharacterType) const
{
	if(CharacterType.EqualTo(CharacterTypeHumanText))
	{
		return HumanTag;
	}
	if(CharacterType.EqualTo(CharacterTypeShamanText))
	{
		return ShamanTag;
	}
	if(CharacterType.EqualTo(ShowAllFilterText))
	{
		return ShowAllTag;
	}
	return FGameplayTag::EmptyTag;
}

FText UUISubsystem::GetCharacterTypeAsTextFromTag(const FGameplayTag& CharacterType) const
{
	if(CharacterType == HumanTag)
	{
		return CharacterTypeHumanText;
	}
	if(CharacterType == ShamanTag)
	{
		return CharacterTypeShamanText;
	}
	if(CharacterType == ShowAllTag)
	{
		return ShowAllFilterText;
	}
	return CharacterTypeShamanText;
}

FGameplayTag UUISubsystem::GetServerModeAsTagFromText(const FText& ServerMode) const
{
	if(ServerMode.EqualTo(ServerModeNormalText))
	{
		return ServerModeNormalTag;
	}
	if(ServerMode.EqualTo(ServerModeTutorialText))
	{
		return ServerModeTutorialTag;
	}
	if(ServerMode.EqualTo(ShowAllFilterText))
	{
		return ShowAllTag;
	}
	return FGameplayTag::EmptyTag;
}

FGameplayTag UUISubsystem::GetServerPrivacyAsTagFromText(const FText& ServerPrivacy) const
{
	if(ServerPrivacy.EqualTo(PrivacyPublicText))
	{
		return PrivacyPublicTag;
	}
	if(ServerPrivacy.EqualTo(PrivacyFriendsOnlyText))
	{
		return PrivacyFriendsOnlyTag;
	}
	if(ServerPrivacy.EqualTo(PrivacyInviteText))
	{
		return PrivacyInviteTag;
	}
	if(ServerPrivacy.EqualTo(PrivacyNoPasswordFilterText))
	{
		return PrivacyNoPasswordTag;
	}
	return  FGameplayTag::EmptyTag;
}

FText UUISubsystem::GetServerModeAsTextFromTag(const FGameplayTag& ServerMode) const
{
	if(ServerMode == ServerModeTutorialTag)
	{
		return ServerModeTutorialText;
	}
	if(ServerMode == ServerModeNormalTag)
	{
		return ServerModeNormalText;
	}
	if(ServerMode == ShowAllTag)
	{
		return ShowAllFilterText;
	}
	return ServerModeNormalText;
}

FText UUISubsystem::GetServerPrivacyAsTextFromTag(const FGameplayTag& ServerPrivacy) const
{
	if(ServerPrivacy == PrivacyPublicTag)
	{
		return PrivacyPublicText;
	}
	if(ServerPrivacy == PrivacyFriendsOnlyTag)
	{
		return PrivacyFriendsOnlyText;
	}
	if(ServerPrivacy == PrivacyInviteTag)
	{
		return PrivacyInviteText;
	}
	if(ServerPrivacy == PrivacyNoPasswordTag)
	{
		return PrivacyNoPasswordFilterText;
	}
	return  PrivacyPublicText;
}

void UUISubsystem::GetInteractionText(EInteractMessageType InteractMessageType, bool bIsHoldingInteraction, FText& OutInteractMessage) const
{
	OutInteractMessage = FText::GetEmpty(); 
	switch (InteractMessageType)
	{
	case EInteractMessageType::IMT_None:
		UE_LOG(LogTemp, Warning , TEXT("%s() THIS SHOULD NOT BE CALLED WITH IMT_NONE"), *FString(__FUNCTION__));
		break;
	case EInteractMessageType::IMT_Pickup:
		if(bIsHoldingInteraction)
		{
			OutInteractMessage = InteractPickupHoldText;
			break;
		}
		OutInteractMessage = InteractPickupPressText;
		break;
	case EInteractMessageType::IMT_Open:
		if(bIsHoldingInteraction)
		{
			OutInteractMessage = InteractOpenHoldText;
			break;
		}
		OutInteractMessage = InteractOpenPressText;
		break;
	default: ;
	}
	
}
