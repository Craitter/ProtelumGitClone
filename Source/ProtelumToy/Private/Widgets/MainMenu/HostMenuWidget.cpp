// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenu/HostMenuWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Subsystems/UISubsystem.h"
#include "Widgets/MainMenu/CharacterSelectionWidget.h"
#include "Widgets/MainMenu/HostSettingsWidget.h"

bool UHostMenuWidget::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	if(CharacterSelection != nullptr)
	{
		const TWeakObjectPtr<UButton> ContinueButton = CharacterSelection->GetContinueButton();
		if(ContinueButton != nullptr)
		{
			ContinueButton->OnClicked.AddDynamic(this, &ThisClass::OpenHostSettings);
		}
	}
	if(HostSettings != nullptr)
	{
		const TWeakObjectPtr<UButton> HostButton = HostSettings->GetHostButton();
		if(HostButton != nullptr)
		{
			HostButton->OnClicked.AddDynamic(this, &ThisClass::Host);
		}
		const TWeakObjectPtr<UButton> CancelButton = HostSettings->GetCancelButton();
		if(CancelButton != nullptr)
		{
			CancelButton->OnClicked.AddDynamic(this, &ThisClass::OpenCharacterSelection);
		}
	}
	const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if(GameInstance.IsValid())
	{
		UISubsystem = GameInstance->GetSubsystem<UUISubsystem>();
	}

	return true;
}

void UHostMenuWidget::OpenHostSettings()
{
	if(Switcher != nullptr && HostSettings != nullptr)
	{
		Switcher->SetActiveWidget(HostSettings);
		HostSettings->SetDefaultServerNamePreview();
	}
}

void UHostMenuWidget::OpenCharacterSelection()
{
	if(Switcher != nullptr && CharacterSelection != nullptr)
	{
		CharacterSelection->DetermineFirstCharacterSelectionScreen();
		Switcher->SetActiveWidget(CharacterSelection);
	}
}

void UHostMenuWidget::Host()
{
	if(!UISubsystem.IsValid() || HostSettings == nullptr)
	{
		return;
	}
	FText LobbyName = HostSettings->GetLobbyName();
	if(LobbyName.IsEmpty())
	{
		LobbyName = UISubsystem->GetDefaultServerName();
	}
	const FText& Password = HostSettings->GetPassword();
	const FText& OutLobbyVisibility = HostSettings->GetLobbyVisibility();
	const FText& OutMode = HostSettings->GetMode();
	const int32 MinLevel = HostSettings->GetMinLevel();
	const int32 MaxLevel = HostSettings->GetMaxLevel();
	const bool bShouldUseVoiceChat = HostSettings->ShouldUseVoiceChat();
	UISubsystem->HostGame(LobbyName, Password, OutLobbyVisibility, OutMode, MinLevel, MaxLevel, bShouldUseVoiceChat);
}
