// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Widgets/ProtelumWidget.h"
#include "LobbySelectionWidget.generated.h"

class UWidgetSwitcher;
class UServerListItemWidget;
class UUISubsystem;
class UButton;
class UCheckBox;
class USpinBox;
class USwitchTextWidget;
/**
 * 
 */

UCLASS()
class PROTELUMTOY_API ULobbySelectionWidget : public UProtelumWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	
	UButton* GetContinueButton() const;

	UButton* GetRefreshButton() const;

	int32 GetSelectedLobbyIndex() const;

	void GetSelectedServerJoinData(FText& CharacterType, FText& LevelRange) const;

	UFUNCTION()
	void PreviousPrivacyText();
	UFUNCTION()
	void NextPrivacyText();

	UFUNCTION()
	void PreviousCharacterType();
	UFUNCTION()
	void NextCharacterType();

	UFUNCTION()
	void PreviousMode();
	UFUNCTION()
	void NextMode();

	UFUNCTION()
	void OnPrivacySwitchChanged();

	UFUNCTION()
	void OnRequiredCharacterTypeSwitchChanged();

	UFUNCTION()
	void OnModeSwitchChanged();

	UFUNCTION()
	void OnAllowedLevelSpinValueChanged(float NewValue);

	UFUNCTION()
	void OnVoiceChatBoxChanged(bool bIsChecked);

	UFUNCTION()
	void OnApplyFilterCheckBoxChanged(bool bIsChecked);

	void SetServerListIsRefreshing(bool bIsRefreshing);

	bool IsSelectedEntryValid() const;

	UFUNCTION()
	void OnListEntrySelected(UServerListItemWidget* SelectedEntry);

	void AddItemToServerList(TObjectPtr<UServerListItemWidget> NewServer);

	void ClearServerList();
private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> ApplyFilter = {nullptr};

	// All, FriendsOnly, NoPassword
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USwitchTextWidget> PrivacySwitch = {nullptr};

	//Human Shaman, all
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USwitchTextWidget> RequiredCharacterTypeSwitch = {nullptr};

	//Tutorial / Normal
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USwitchTextWidget> GameModeSwitch = {nullptr};
	// all - min -  max - all
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> AllowedLevelSpin = {nullptr};

	//Checked, Unchecked
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> VoiceChatBox = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ContinueButton = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RefreshServerListButton = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> ServerListSwitcher = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ServerListLoadingWidget = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ServerList = {nullptr};

	TWeakObjectPtr<UServerListItemWidget> SelectedServerListEntry = {nullptr};

	TSubclassOf<UServerListItemWidget> ServerListItemClass = {nullptr};

	TWeakObjectPtr<UUISubsystem> UISubsystem = {nullptr};
};
