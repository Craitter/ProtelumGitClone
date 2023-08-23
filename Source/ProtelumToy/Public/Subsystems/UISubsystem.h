// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ActorComponents/ContainerComponent.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Widgets/ProtelumUserInterfaceBaseWidget.h"
#include "Widgets/UtilityWidgets/MessageTypes.h"
#include "Widgets/UtilityWidgets/ServerTypes.h"
#include "UISubsystem.generated.h"

class UContainerSlotWidget;
enum class EInteractMessageType;
class UGameOverlayWidget;
struct FServerSearchSettings;
struct FHostSettings;
class UPasswordPromptWidget;
class UServerListItemWidget;
class UProtelumWidget;
class UProtelumGameInstance;
class UProtelumUserInterfaceBaseWidget;
class UPromptWidget;
class UMainMenuWidget;
class USaveGameSubsystem;


/** Todo: UISubsystem:
 * Todo: 1. Navigation inside the menus with controller
 * Todo: 2. Control the active Input mode (Input Mapping contexts) and mouse control
 * Todo: 4. Maybe Prompts more referring to the error source with localizable {value} parameters (ask marvin)
 */
DECLARE_MULTICAST_DELEGATE(FOnServerListReadyDelegate)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnServerListEntryCreatedDelegate, TObjectPtr<UServerListItemWidget>)
DECLARE_MULTICAST_DELEGATE(FOnOpenCharacterSelectionMenu)
DECLARE_MULTICAST_DELEGATE(FOnServerListFilterChanged)

UCLASS()
class PROTELUMTOY_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UUISubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void LoadInterfaceBaseWidget(EBaseWidgetType WidgetType);
	void QuitGame() const;

	
	FString GetUsername() const;
	FText GetDefaultServerName() const;
	
	//Prompt Begin
	void EnablePrompt(TWeakObjectPtr<UUserWidget> PromptToFocus) const;
	FOnPlayerPromptChoice* CreatePrompt(EPromptType PromptType, EPromptMessageType ErrorMessageType, bool& WasSuccessful) const;
	FOnPasswordEnteringConfirmedDelegate* CreatePasswordPrompt() const;
	FText CreatePromptMessage(EPromptMessageType ErrorMessageType, FText& PromptHeader) const;
	UFUNCTION()
	void OnPromptChoiceMade(EPromptReply Reply) const;
	void DisablePrompt() const;
	//Prompt Ends

	//Host Begin
	void HostGame(const FText& LobbyName, const FText& Password, const FText& InServerVisibility, const FText& InServerMode, const int32
			  MinLevel, const int32 MaxLevel, const bool bInVoiceChat) const;
	FText GetPreviousServerPrivacy(const FText& CurrentLobbyVisibility, const bool bIsHostPrivacy) const;
	FText GetNextServerPrivacy(const FText& CurrentServerPrivacy, bool bIsHostPrivacy) const;
	FText GetPreviousServerMode(const FText& CurrentServerMode, bool bCanBeAll = false) const;
	FText GetNextServerMode(const FText& CurrentServerMode, bool bCanBeAll = false) const;
	//Host Ends
	
	//Join Begin
	const FText& GetJoinText();
	bool GetIsServerListRefreshing() const;
	void RefreshServerList();
	void JoinGame(int32 Index) const;
	bool IsServerStillValid(int32 Index) const;
	bool IsServerPasswordProtected(int32 ServerListIndex) const;
	bool IsPasswordCorrect(int32 ServerListIndex, const FString& ServerPassword) const;
	bool IsCharacterValidForSelectedServer(int32 ServerListIndex) const;
	//Join Ends

	//Server Search Begin
	void SetServerSearchIsResultFiltered(bool bShouldFilter);
	void SetServerSearchPrivacyFilter(const FText& SearchPrivacy);
	void SetServerSearchRequiredCharacter(const FText& RequiredCharacter);
	void SetServerSearchServerModeFilter(const FText& ServerModeFilter);
	void SetServerSearchAllowedLevelFilter(const uint32 InAllowedLevel);
	void SetServerSearchVoiceChatFilter(bool bIsVoiceChatAllowed);

	FOnServerListFilterChanged* GetOnServerListFilterChanged();
	FOnServerListEntryCreatedDelegate* GetOnServerListItemCreatedDelegate();
	FOnServerListReadyDelegate* GetOnServerListReadyDelegate();
	
	UFUNCTION()
	void OnSearchResultsReady(const TArray<FHostSettings>& InFoundSessionsHostData);
	//Server Search Ends

	//Character Selection Begin
	FText GetPreviousCharacterType(const FText& CurrentType, bool bCanBeAll = false) const;
	FText GetNextCharacterType(const FText& CurrentType, bool bCanBeAll = false) const;
	FText CreateCharacter(const FText& CharacterName, const FText& CharacterType, bool& bWasSuccessful) const;
	FText DeleteCharacter(bool& bWasLastCharacter) const;
	FText GetActiveCharacter() const;
	FText GetNextCharacter() const;
	FText GetPreviousCharacter() const;
	//Character Selection Ends

	//Character SaveGame Information Begin
	FText GetCharacterTypeDescription(const FText& CurrentType) const; //Todo: Localize this display text as well as stats on stats menu
	FText GetCharacterType() const;
	FText GetCharacterLevel() const;
	FText GetCharacterCraftedArtifactsCount() const;
	FText GetCharacterLastPlayedWith() const;
	FText GetCharacterPlayTimeInHours() const;
	//Character SaveGame Information Ends


	//Interact Begin
	UFUNCTION(BlueprintCallable)
	void ShowInteractPrompt(float TimeToHold, FInteractMessageInformation InteractMessageInformation) const;
	UFUNCTION(BlueprintCallable)
	void HideInteractPrompt() const;
	UFUNCTION(BlueprintCallable)
	void StartInteractionTimer(float TimeToHold) const;
	UFUNCTION(BlueprintCallable)
	void StopInteractionTimer() const;
	
	//Interact Ends

	// void OpenGameHUD();
	// void OpenPauseMenu();
	//Inventory Begin
	UFUNCTION(BlueprintCallable)
	bool OpenInventory(
		UContainerComponent* InParentContainer) const;
	void OpenInventoryExpanded();

	void ContainerSlotClicked(TWeakObjectPtr<UContainerSlotWidget> InContainerSlotWidget, FKey ImpactKey);
	// void StartContainerDragDrop(TWeakObjectPtr<UContainerSlotWidget> ContainerSlot);
	//Inventory Ends

	EBaseWidgetType BaseWidgetType = EBaseWidgetType::None;

	void InitMenuInput();
	void InitHideoutInput();
	void InitMissionInput();

	static FVector2D GetGameResolution();
	static FVector2D GetViewportSize();
private:
	void SetGameOnly() const;
	void SetMenuInput(TWeakObjectPtr<UUserWidget> NewWidget) const;



	TSubclassOf<UProtelumUserInterfaceBaseWidget> GameOverlayWidgetClass = {nullptr};
	TSubclassOf<UProtelumUserInterfaceBaseWidget> MainMenuWidgetClass = {nullptr};
	TSubclassOf<UProtelumUserInterfaceBaseWidget> HideoutWidgetClass = {nullptr};
	
	UPROPERTY()
	TObjectPtr<UProtelumUserInterfaceBaseWidget> UserInterfaceBase = {nullptr};
	TObjectPtr<UGameOverlayWidget> GameOverlayWidget = {nullptr};

	//Inventory Begin
	TWeakObjectPtr<UContainerSlotWidget> CachedSlot = {nullptr};
	TSubclassOf<UUserWidget> ContainerGridWidgetClass = {nullptr};
	TSubclassOf<UUserWidget> DragDropWidgetClass = {nullptr};
	//Inventory Ends
	
	//Prompt Begin
	TSubclassOf<UUserWidget> GenericPromptClass = {nullptr};
	TSubclassOf<UPasswordPromptWidget> PasswordPromptClass = {nullptr};
	//Prompt Ends
	
	//Server Search Filter Begin
	FServerSearchSettings ActiveSearchSettings;
	TArray<FHostSettings> FoundSessionsHostData;
	
	TSubclassOf<UServerListItemWidget> ServerListItemClass;

	bool bIsServerListRefreshing = false;
	FOnServerListFilterChanged OnServerListFilterChanged;
	FOnServerListReadyDelegate OnServerListReadyDelegate;
	void UpdateShownServerList();

	bool MeetsPrivacyFilter(const FHostSettings& HostData) const;
	bool MeetsCharacterTypeFilter(const FHostSettings& HostData) const;
	bool MeetsServerModeFilter(const FHostSettings& HostData) const;
	bool MeetsAllowedLevelFilter(const FHostSettings& HostData) const;
	bool MeetsVoiceChatFilter(const FHostSettings& HostData) const;
	//Server Search Filter Ends
	
	//Display ServerListEntry Begin
	
	TWeakObjectPtr<UTexture2D> GetIsPasswordLockedImage(const FString& Password);
	TWeakObjectPtr<UTexture2D> GetIsVoiceChatEnabledImage(bool bIsVoiceChatEnabled);
	FText GetLevelRangeAsText(uint32 MinLevel, uint32 MaxLevel) const;
	
	
	void CreateServerListEntry(const FServerListEntryData& ListEntryData) const;
	FOnServerListEntryCreatedDelegate OnServerListEntryCreatedDelegate;
	//Display ServerListEntry Ends

	FGameplayTag ShowAllTag;

	//Tag - Text Helper Functions Begin
	FGameplayTag GetCharacterTypeAsTagFromText(const FText& CharacterType) const;
	FGameplayTag GetServerModeAsTagFromText(const FText& ServerMode) const;
	FGameplayTag GetServerPrivacyAsTagFromText(const FText& ServerPrivacy) const;

	FText GetCharacterTypeAsTextFromTag(const FGameplayTag& CharacterType) const;
	FText GetServerModeAsTextFromTag(const FGameplayTag& ServerMode) const;
	FText GetServerPrivacyAsTextFromTag(const FGameplayTag& ServerPrivacy) const;
	//Tag - Text Helper Functions Ends

	//Server Modes Begin
	FGameplayTag ServerModeTutorialTag;
	FText ServerModeTutorialText = FText::GetEmpty();
	
	FGameplayTag ServerModeNormalTag;
	FText ServerModeNormalText = FText::GetEmpty();
	//Server Modes Ends
	
	//Character Types Begin
	FGameplayTag HumanTag = FGameplayTag::EmptyTag;
	FText CharacterTypeHumanText = FText::GetEmpty();
	FText CharacterTypeDescriptionHumanText = FText::GetEmpty();
	
	FGameplayTag ShamanTag = FGameplayTag::EmptyTag;
	FText CharacterTypeShamanText = FText::GetEmpty();
	FText CharacterTypeDescriptionShamanText = FText::GetEmpty();
	//Character Types Ends
	
	//Begin PromptMessages
	FText PromptHeaderQuitGameText = FText::GetEmpty();
	FText PromptQuitGameText = FText::GetEmpty();
	
	FText PromptHeaderReturnMainMenuText = FText::GetEmpty();
	FText PromptReturnMainMenuText = FText::GetEmpty();

	FText PromptHeaderDeleteCharacterText = FText::GetEmpty();
	FText PromptDeleteCharacterText = FText::GetEmpty();
	
	FText PromptHeaderCharacterCreationFailedText = FText::GetEmpty();
	FText PromptNoSymbolsText = FText::GetEmpty();
	FText PromptDuplicatedNameText = FText::GetEmpty();
	FText PromptMaxCharacterCountText = FText::GetEmpty();
	FText PromptInvalidNameSizeText = FText::GetEmpty();

	FText PromptHeaderInvalidServerText = FText::GetEmpty();
	FText PromptInvalidServerText = FText::GetEmpty();

	FText PromptHeaderNoServerSelectedText = FText::GetEmpty();
	FText PromptNoServerSelectedText = FText::GetEmpty();

	FText PromptHeaderCharacterMismatchesServerRequirementsText = FText::GetEmpty();
	FText PromptCharacterMismatchesServerRequirementsText = FText::GetEmpty();
	//End PromptMessages

	//Privacy Text Begin
	FGameplayTag PrivacyPublicTag = FGameplayTag::EmptyTag;
	FText PrivacyPublicText = FText::GetEmpty();
	FGameplayTag PrivacyInviteTag = FGameplayTag::EmptyTag;
	FText PrivacyInviteText = FText::GetEmpty();
	FGameplayTag PrivacyFriendsOnlyTag = FGameplayTag::EmptyTag;
	FText PrivacyFriendsOnlyText = FText::GetEmpty();
	FGameplayTag PrivacyNoPasswordTag = FGameplayTag::EmptyTag;
	FText PrivacyNoPasswordFilterText = FText::GetEmpty();
	//Privacy Text Ends

	//Additional Texts Begin
	FText JoinText = FText::GetEmpty();
	FText ShowAllFilterText = FText::GetEmpty();
	
	// {min_level} - {max_level}
	FText LevelRangeText = FText::GetEmpty();

	//{username}
	FText DefaultServerName = FText::GetEmpty();;

	//Additional Texts Ends

	//Interact Begin
	void GetInteractionText(EInteractMessageType InteractMessageType, bool bIsHoldingInteraction, FText& OutInteractMessage) const;
	//{key} {noun}
	FText InteractPickupHoldText = FText::GetEmpty();
	//{key} {noun}
	FText InteractPickupPressText = FText::GetEmpty();

	//{key} {noun}
	FText InteractOpenHoldText = FText::GetEmpty();
	//{key} {noun}
	FText InteractOpenPressText = FText::GetEmpty();
	
	//Interact Ends

	
	TWeakObjectPtr<USaveGameSubsystem> SaveGameSubsystem = {nullptr};
	TWeakObjectPtr<UProtelumGameInstance> ProtelumGameInstance = {nullptr};
};
