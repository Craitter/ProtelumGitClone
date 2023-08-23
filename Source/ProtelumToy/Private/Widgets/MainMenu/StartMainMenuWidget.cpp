// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenu/StartMainMenuWidget.h"

#include "Components/Button.h"
#include "..\..\..\Public\Subsystems\UISubsystem.h"
#include "Widgets/MainMenu/InformationPanelWidget.h"

bool UStartMainMenuWidget::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	
	if(!ensure(QuitGameButton != nullptr)) return false;
	QuitGameButton->OnClicked.AddDynamic(this, &UStartMainMenuWidget::OpenQuitGamePrompt);

	if(InformationPanel != nullptr)
	{
		PlayAnimation(ShowInformationPanel, 1.0f, 1, EUMGSequencePlayMode::Reverse);
		const TWeakObjectPtr<UButton> InfoButton = InformationPanel->GetInformationButton();
		if(InfoButton.IsValid())
		{
			InfoButton->OnClicked.AddDynamic(this, &ThisClass::UStartMainMenuWidget::OpenInformationPanel);
		}
		
	}
	
	return true;
}

void UStartMainMenuWidget::OpenInformationPanel()
{
	if(bInfoPanelIsOpen)
	{
		PlayAnimationReverse(ShowInformationPanel);
		bInfoPanelIsOpen = false;
		return;
	}
	bInfoPanelIsOpen = true;
	PlayAnimationForward(ShowInformationPanel);
}

void UStartMainMenuWidget::OpenQuitGamePrompt()
{
	const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if(GameInstance.IsValid())
	{
		const TWeakObjectPtr<UUISubsystem> UISubsystem = GameInstance->GetSubsystem<UUISubsystem>();
		if(UISubsystem.IsValid())
		{
			bool bSuccess;
			FOnPlayerPromptChoice* PromptChoice = UISubsystem->CreatePrompt(EPromptType::YesNoPrompt, EPromptMessageType::QuitGame, bSuccess);
			if(bSuccess && PromptChoice != nullptr)
			{
				PromptChoice->AddUObject(this, &ThisClass::OnPromptChoiceMade);
			}
		}
	}
}

TWeakObjectPtr<UButton> UStartMainMenuWidget::GetHostButton() const
{
	return HostButton;
}

TWeakObjectPtr<UButton>UStartMainMenuWidget::GetJoinButton() const
{
	return JoinButton;
}

TWeakObjectPtr<UButton> UStartMainMenuWidget::GetSettingsButton() const
{
	return SettingsButton;
}

TWeakObjectPtr<UButton> UStartMainMenuWidget::GetStatsButton() const
{
	return StatsButton;
}

void UStartMainMenuWidget::OnPromptChoiceMade(EPromptReply PromptReply) const
{
	if(PromptReply == EPromptReply::Yes)
	{
		const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
		if(GameInstance.IsValid())
		{
			const TWeakObjectPtr<UUISubsystem> UISubsystem = GameInstance->GetSubsystem<UUISubsystem>();
			if(UISubsystem.IsValid())
			{
				UISubsystem->QuitGame();
			}
		}
		return;
	}
}


