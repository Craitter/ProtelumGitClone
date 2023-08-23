// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenu/MainMenuWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Subsystems/UISubsystem.h"
#include "Widgets/MainMenu/HostMenuWidget.h"
#include "Widgets/MainMenu/JoinMenuWidget.h"
#include "Widgets/MainMenu/SettingsMenuWidget.h"
#include "Widgets/MainMenu/StartMainMenuWidget.h"
#include "Widgets/MainMenu/StatsMenu.h"

void UMainMenuWidget::OpenStartMenu()
{
	NextWidget = StartMenu.Get();
	HideActiveWidget();
}

void UMainMenuWidget::OpenHostMenu()
{
	if(HostMenu != nullptr)
	{
		HostMenu->OpenCharacterSelection();
	}
	NextWidget = HostMenu.Get();
	HideActiveWidget();
}

void UMainMenuWidget::OpenJoinMenu()
{
	if(JoinMenu != nullptr)
	{
		JoinMenu->OpenLobbySelection();
		//JoinMenu-> CacheCharacterSelection();
	}
	NextWidget = JoinMenu.Get();
	HideActiveWidget();
}

void UMainMenuWidget::OpenSettingsMenu()
{
	NextWidget = SettingsMenu.Get();
	HideActiveWidget();
}

void UMainMenuWidget::OpenStatsMenu()
{
	NextWidget = StatsMenu.Get();
	HideActiveWidget();
}



bool UMainMenuWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if(!bSuccess)
	{
		return bSuccess;
	}

	WidgetType = EBaseWidgetType::MainMenu;
	
	const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if(GameInstance.IsValid())
	{
		UISubsystem = GameInstance->GetSubsystem<UUISubsystem>();
	}
	

	OnSwitchAnimEnds.BindDynamic(this, &ThisClass::RevealActiveWidget);
	
	if(StartMenu != nullptr)
	{
		const TWeakObjectPtr<UButton> HostButton = StartMenu->GetHostButton();
		if(HostButton.IsValid())
		{
			HostButton->OnClicked.AddDynamic(this, &ThisClass::OpenHostMenu);
		}
		const TWeakObjectPtr<UButton> JoinButton = StartMenu->GetJoinButton();
		if(JoinButton.IsValid())
		{
			JoinButton->OnClicked.AddDynamic(this, &ThisClass::OpenJoinMenu);
		}
		const TWeakObjectPtr<UButton> SettingsButton = StartMenu->GetSettingsButton();
		if(SettingsButton.IsValid())
		{
			SettingsButton->OnClicked.AddDynamic(this, &ThisClass::OpenSettingsMenu);
		}
		const TWeakObjectPtr<UButton> StatsButton = StartMenu->GetStatsButton();
		if(StatsButton.IsValid())
		{
			StatsButton->OnClicked.AddDynamic(this, &ThisClass::OpenStatsMenu);
		}
	}
	if(ReturnButton != nullptr)
	{
		ReturnButton->OnClicked.AddDynamic(this, &ThisClass::OpenStartMenu);
	}
	NextWidget = StartMenu;
	RevealActiveWidget();
	
	return bSuccess;
}



void UMainMenuWidget::HideActiveWidget()
{
	if(bIsSwitching)
	{
		return;
	}
	bIsSwitching = true;	
	BindToAnimationFinished(SwitchAwayAnim, OnSwitchAnimEnds);
	PlayAnimationForward(SwitchAwayAnim);
}

void UMainMenuWidget::RevealActiveWidget()
{
	if(Switcher != nullptr)
	{
		Switcher->SetActiveWidget(NextWidget.Get());
	}
	if(ReturnButton != nullptr)
	{
		if(NextWidget != StartMenu)
		{
			ReturnButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ReturnButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	UnbindFromAnimationFinished(SwitchAwayAnim, OnSwitchAnimEnds);
	PlayAnimation(SwitchAwayAnim, 0, 1, EUMGSequencePlayMode::Reverse);
	bIsSwitching = false;
}
