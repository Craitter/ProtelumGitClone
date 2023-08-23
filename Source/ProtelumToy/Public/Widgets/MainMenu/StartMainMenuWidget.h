// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterSelectionWidget.h"
#include "Widgets/ProtelumWidget.h"
#include "StartMainMenuWidget.generated.h"

// enum EPromptReply : int;
class UButton;
class UInformationPanelWidget;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UStartMainMenuWidget : public UProtelumWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;


	TWeakObjectPtr<UButton> GetHostButton() const;
	TWeakObjectPtr<UButton> GetJoinButton() const;
	TWeakObjectPtr<UButton> GetSettingsButton() const;
	TWeakObjectPtr<UButton> GetStatsButton() const;

	void OnPromptChoiceMade(EPromptReply PromptReply) const;
protected:
	UFUNCTION()
	void OpenInformationPanel();
	UFUNCTION()
	void OpenQuitGamePrompt();


private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> HostButton = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> JoinButton = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SettingsButton = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StatsButton = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitGameButton = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInformationPanelWidget> InformationPanel = {nullptr};
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShowInformationPanel = {nullptr};

	bool bInfoPanelIsOpen = false;
};
