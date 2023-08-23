// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ProtelumWidget.h"
#include "JoinMenuWidget.generated.h"

class UPasswordPromptWidget;
enum class EPromptReply;
class UButton;
class UUISubsystem;
class UCharacterSelectionWidget;
class ULobbySelectionWidget;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UJoinMenuWidget : public UProtelumWidget
{
	GENERATED_BODY()
public:
	
	virtual bool Initialize() override;

	UFUNCTION()
	void OpenLobbySelection();

	UFUNCTION()
	void OpenCharacterSelection();
	
	UFUNCTION()
	void Join();	

	UFUNCTION()
	void OnLobbySelectionConfirmed();

	UFUNCTION()
	void RefreshServerList();

	UFUNCTION()
	void ServerListRefreshed() const;

	bool OnTryPasswordConfirmation(EPromptReply PromptReply, const FString& ServerPassword);
private:
	bool IsServerSelected() const;

	bool IsServerStillValid() const;

	bool IsServerPasswordProtected() const;

	int32 SelectedLobbyIndex = -1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CancelJoinButton = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<ULobbySelectionWidget> LobbySelection = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCharacterSelectionWidget> CharacterSelection = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> CharacterSelectionWidget = {nullptr};

	TWeakObjectPtr<UUISubsystem> UISubsystem = {nullptr};
};
