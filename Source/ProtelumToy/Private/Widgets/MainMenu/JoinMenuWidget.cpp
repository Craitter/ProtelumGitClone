// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenu/JoinMenuWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Subsystems/UISubsystem.h"
#include "Widgets/MainMenu/CharacterSelectionWidget.h"
#include "Widgets/MainMenu/LobbySelectionWidget.h"


bool UJoinMenuWidget::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if(GameInstance.IsValid())
	{
		UISubsystem = GameInstance->GetSubsystem<UUISubsystem>();
	}
	if(UISubsystem.IsValid())
	{
		FOnServerListReadyDelegate* OnServerListReady = UISubsystem->GetOnServerListReadyDelegate();
		if(OnServerListReady != nullptr)
		{
			OnServerListReady->AddUObject(this, &ThisClass::ServerListRefreshed);
		}
	}
	
	if(LobbySelection != nullptr)
	{
		const TWeakObjectPtr<UButton> ContinueButton = LobbySelection->GetContinueButton();
		if(ContinueButton.IsValid())
		{
			ContinueButton->OnClicked.AddDynamic(this, &ThisClass::OnLobbySelectionConfirmed);
		}
		const TWeakObjectPtr<UButton> RefreshServerListButton = LobbySelection->GetRefreshButton();
		if(RefreshServerListButton != nullptr)
		{
			RefreshServerListButton->OnClicked.AddDynamic(this, &ThisClass::RefreshServerList);
		}
	}
	if(CharacterSelection != nullptr)
	{
		const TWeakObjectPtr<UButton> ContinueButton = CharacterSelection->GetContinueButton();
		if(ContinueButton.IsValid())
		{
			ContinueButton->OnClicked.AddDynamic(this, &ThisClass::Join);
		}
		const TWeakObjectPtr<UTextBlock> ContinueText = CharacterSelection->GetContinueText();
		if(ContinueText.IsValid() && UISubsystem.IsValid())
		{
			ContinueText->SetText(UISubsystem->GetJoinText());
		}
	}
	if(CancelJoinButton != nullptr)
	{
		CancelJoinButton->OnClicked.AddDynamic(this, &ThisClass::OpenLobbySelection);
	}

	return true;
}

void UJoinMenuWidget::OpenLobbySelection()
{
	if(Switcher != nullptr)
	{
		Switcher->SetActiveWidget(LobbySelection);
	}
	RefreshServerList();
}

void UJoinMenuWidget::OpenCharacterSelection()
{
	if(Switcher == nullptr || LobbySelection == nullptr || !UISubsystem.IsValid() || CharacterSelection == nullptr)
	{
		return;
	}

	FText CharacterType;
	FText LevelRange;
	
	LobbySelection->GetSelectedServerJoinData(CharacterType, LevelRange);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Hello %s %s"), *CharacterType.ToString(), *LevelRange.ToString()));
	CharacterSelection->SetServerRequirements(CharacterType, LevelRange);
	CharacterSelection->DetermineFirstCharacterSelectionScreen();
	Switcher->SetActiveWidget(CharacterSelectionWidget);
}

void UJoinMenuWidget::Join()
{
	if(!UISubsystem.IsValid() || LobbySelection == nullptr)
	{
		return;
	}
	if(!UISubsystem->IsServerStillValid(LobbySelection->GetSelectedLobbyIndex()))
	{
		bool Success;
		UISubsystem->CreatePrompt(EPromptType::OkPrompt, EPromptMessageType::ServerIsNotValid, Success);
		OpenLobbySelection();
		return;
	}
	if(!UISubsystem->IsCharacterValidForSelectedServer(LobbySelection->GetSelectedLobbyIndex()))
	{
		bool Success;
		UISubsystem->CreatePrompt(EPromptType::OkPrompt, EPromptMessageType::CharacterNotMatchingRequirements, Success);
		return;
	}
	UISubsystem->JoinGame(LobbySelection->GetSelectedLobbyIndex());
}

void UJoinMenuWidget::OnLobbySelectionConfirmed()
{
	if(!UISubsystem.IsValid())
	{
		return;
	}
	if(!IsServerSelected())
	{
		bool Success;
		UISubsystem->CreatePrompt(EPromptType::OkPrompt, EPromptMessageType::NoServerSelected, Success);
		return;
	}
	if(!IsServerStillValid())
	{
		bool Success;
		UISubsystem->CreatePrompt(EPromptType::OkPrompt, EPromptMessageType::ServerIsNotValid, Success); //Not valid anymore automatically reloading server list
		RefreshServerList();
		return;
	}
	if(IsServerPasswordProtected())
	{
		FOnPasswordEnteringConfirmedDelegate* OnPasswordEnteringConfirmedDelegate = UISubsystem->CreatePasswordPrompt();
		if(OnPasswordEnteringConfirmedDelegate != nullptr)
		{
			OnPasswordEnteringConfirmedDelegate->BindUObject(this, &ThisClass::OnTryPasswordConfirmation);
		}
		return;
	}
	OpenCharacterSelection();
}

void UJoinMenuWidget::RefreshServerList()
{
	if(UISubsystem.IsValid() && !UISubsystem->GetIsServerListRefreshing() && LobbySelection != nullptr)
	{
		UISubsystem->RefreshServerList();
		LobbySelection->SetServerListIsRefreshing(true);
	}
}

void UJoinMenuWidget::ServerListRefreshed() const
{
	if(!UISubsystem.IsValid() || LobbySelection == nullptr)
	{
		return;
	}
	LobbySelection->SetServerListIsRefreshing(false);
}

bool UJoinMenuWidget::OnTryPasswordConfirmation(EPromptReply PromptReply, const FString& ServerPassword)
{
	
	if(UISubsystem.IsValid() && LobbySelection != nullptr)
	{
		if(PromptReply == EPromptReply::Cancel)
		{
			UISubsystem->DisablePrompt();
			return true;
		}
		if(UISubsystem->IsPasswordCorrect(LobbySelection->GetSelectedLobbyIndex(), ServerPassword))
		{
			UISubsystem->DisablePrompt();
			OpenCharacterSelection();
			
			return true;
		}
	}
	return false;
}


bool UJoinMenuWidget::IsServerSelected() const
{
	if(LobbySelection != nullptr && LobbySelection->IsSelectedEntryValid())
	{
		return true;
	}
	return false;
}

bool UJoinMenuWidget::IsServerStillValid() const
{
	if(UISubsystem.IsValid() && LobbySelection != nullptr && LobbySelection->IsSelectedEntryValid() && UISubsystem->IsServerStillValid(LobbySelection->GetSelectedLobbyIndex()))
	{
		return true;
	}
	return false;
}

bool UJoinMenuWidget::IsServerPasswordProtected() const
{
	if(UISubsystem.IsValid() && LobbySelection != nullptr && LobbySelection->IsSelectedEntryValid() && UISubsystem->IsServerPasswordProtected(LobbySelection->GetSelectedLobbyIndex()))
	{
		return true;
	}
	return false;
}
