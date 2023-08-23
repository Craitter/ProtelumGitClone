// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenu/LobbySelectionWidget.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/SpinBox.h"
#include "Components/WidgetSwitcher.h"
#include "ProtelumToy/ProtelumToy.h"
#include "Subsystems/UISubsystem.h"
#include "Widgets/MainMenu/ServerListItemWidget.h"
#include "Widgets/UtilityWidgets/SwitchTextWidget.h"

bool ULobbySelectionWidget::Initialize()
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
		FOnServerListEntryCreatedDelegate* OnServerListItemCreated = UISubsystem->GetOnServerListItemCreatedDelegate();
		if(OnServerListItemCreated != nullptr)
		{
			OnServerListItemCreated->AddUObject(this, &ThisClass::AddItemToServerList);
		}

		FOnServerListFilterChanged* OnServerListFilterChanged = UISubsystem->GetOnServerListFilterChanged();
		if(OnServerListFilterChanged != nullptr)
		{
			OnServerListFilterChanged->AddUObject(this, &ThisClass::ClearServerList);
		}
	}
	
	if(PrivacySwitch != nullptr)
	{
		const TWeakObjectPtr<UButton> NextPrivacyButton = PrivacySwitch->GetDisplayNextTextButton();
		if(NextPrivacyButton.IsValid())
		{
			NextPrivacyButton->OnClicked.AddDynamic(this, &ThisClass::NextPrivacyText);
		}

		const TWeakObjectPtr<UButton> PreviousPrivacyButton = PrivacySwitch->GetDisplayPreviousTextButton();
		if(PreviousPrivacyButton.IsValid())
		{
			PreviousPrivacyButton->OnClicked.AddDynamic(this, &ThisClass::PreviousPrivacyText);
		}
		if(UISubsystem.IsValid())
		{
			PrivacySwitch->ForceDisplayedText(UISubsystem->GetNextServerPrivacy(FText::GetEmpty(), false));
		}
		PrivacySwitch->OnDisplayedTextChanged.AddUObject(this, &ThisClass::OnPrivacySwitchChanged);
		OnPrivacySwitchChanged();
	}
	if(RequiredCharacterTypeSwitch != nullptr)
	{
		const TWeakObjectPtr<UButton> NextCharacterTypeButton = RequiredCharacterTypeSwitch->GetDisplayNextTextButton();
		if(NextCharacterTypeButton.IsValid())
		{
			NextCharacterTypeButton->OnClicked.AddDynamic(this, &ThisClass::NextCharacterType);
		}

		const TWeakObjectPtr<UButton> PreviousCharacterTypeButton = RequiredCharacterTypeSwitch->GetDisplayPreviousTextButton();
		if(PreviousCharacterTypeButton.IsValid())
		{
			PreviousCharacterTypeButton->OnClicked.AddDynamic(this, &ThisClass::PreviousCharacterType);
		}
		if(UISubsystem.IsValid())
		{
			RequiredCharacterTypeSwitch->ForceDisplayedText(UISubsystem->GetNextCharacterType(FText::GetEmpty(), true));
		}
		RequiredCharacterTypeSwitch->OnDisplayedTextChanged.AddUObject(this, &ThisClass::OnRequiredCharacterTypeSwitchChanged);
		OnRequiredCharacterTypeSwitchChanged();
	}
	if(GameModeSwitch != nullptr)
	{
		const TWeakObjectPtr<UButton> NextModeButton = GameModeSwitch->GetDisplayNextTextButton();
		if(NextModeButton.IsValid())
		{
			NextModeButton->OnClicked.AddDynamic(this, &ThisClass::NextMode);
		}

		const TWeakObjectPtr<UButton> PreviousModeButton = GameModeSwitch->GetDisplayPreviousTextButton();
		if(PreviousModeButton.IsValid())
		{
			PreviousModeButton->OnClicked.AddDynamic(this, &ThisClass::PreviousMode);
		}
		if(UISubsystem.IsValid())
		{
			GameModeSwitch->ForceDisplayedText(UISubsystem->GetNextServerMode(FText::GetEmpty(), true));
		}
		GameModeSwitch->OnDisplayedTextChanged.AddUObject(this, &ThisClass::OnModeSwitchChanged);
		OnModeSwitchChanged();
	}

	if(AllowedLevelSpin != nullptr)
	{
		AllowedLevelSpin->SetMaxValue(CHARACTER_LEVEL_MAX);
		AllowedLevelSpin->SetMinValue(CHARACTER_LEVEL_MIN);
		AllowedLevelSpin->MaxFractionalDigits = 0.0f;
		AllowedLevelSpin->MinFractionalDigits = 0.0f;
		AllowedLevelSpin->Delta = 1.0f;
		AllowedLevelSpin->SetValue(CHARACTER_LEVEL_MIN);
		AllowedLevelSpin->OnValueChanged.AddDynamic(this, &ThisClass::OnAllowedLevelSpinValueChanged);
		OnAllowedLevelSpinValueChanged(CHARACTER_LEVEL_MIN);
	}
	if(VoiceChatBox != nullptr)
	{
		VoiceChatBox->SetCheckedState(ECheckBoxState::Checked);
		VoiceChatBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnVoiceChatBoxChanged);
		OnVoiceChatBoxChanged(true);
	}
	if(ApplyFilter != nullptr)
	{
		ApplyFilter->SetCheckedState(ECheckBoxState::Unchecked);
		ApplyFilter->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnApplyFilterCheckBoxChanged);
		OnApplyFilterCheckBoxChanged(false);
	}
	
	return true;
}


UButton* ULobbySelectionWidget::GetContinueButton() const
{
	return ContinueButton;
}

UButton* ULobbySelectionWidget::GetRefreshButton() const
{
	return RefreshServerListButton;
}

int32 ULobbySelectionWidget::GetSelectedLobbyIndex() const
{
	if(SelectedServerListEntry.IsValid())
	{
		return SelectedServerListEntry->GetServerIndex();
	}
	return -1;
}

void ULobbySelectionWidget::GetSelectedServerJoinData(FText& CharacterType, FText& LevelRange) const
{
	if(SelectedServerListEntry != nullptr)
	{
		CharacterType = SelectedServerListEntry->GetCharacterTypeAsText();

		LevelRange = SelectedServerListEntry->GetLevelRangeAsText();
	}
}

void ULobbySelectionWidget::PreviousPrivacyText()
{
	if(!UISubsystem.IsValid() || PrivacySwitch == nullptr)
	{
		return;
	}
	FText CurrentSearchPrivacy = PrivacySwitch->GetCurrentlyDisplayedText();
	CurrentSearchPrivacy = UISubsystem->GetPreviousServerPrivacy(CurrentSearchPrivacy, false);
	PrivacySwitch->ShowPreviousText(CurrentSearchPrivacy);
}

void ULobbySelectionWidget::NextPrivacyText()
{
	if(!UISubsystem.IsValid() || PrivacySwitch == nullptr)
	{
		return;
	}
	FText CurrentSearchPrivacy = PrivacySwitch->GetCurrentlyDisplayedText();
	CurrentSearchPrivacy = UISubsystem->GetNextServerPrivacy(CurrentSearchPrivacy, false);
	PrivacySwitch->ShowNextText(CurrentSearchPrivacy);
}

void ULobbySelectionWidget::PreviousCharacterType()
{
	if(!UISubsystem.IsValid() || RequiredCharacterTypeSwitch == nullptr)
	{
		return;
	}
	FText CurrentCharacterType = RequiredCharacterTypeSwitch->GetCurrentlyDisplayedText();
	CurrentCharacterType = UISubsystem->GetPreviousCharacterType(CurrentCharacterType, true);
	RequiredCharacterTypeSwitch->ShowPreviousText(CurrentCharacterType);
}

void ULobbySelectionWidget::NextCharacterType()
{
	if(!UISubsystem.IsValid() || RequiredCharacterTypeSwitch == nullptr)
	{
		return;
	}
	FText CurrentCharacterType = RequiredCharacterTypeSwitch->GetCurrentlyDisplayedText();
	CurrentCharacterType = UISubsystem->GetNextCharacterType(CurrentCharacterType, true);
	RequiredCharacterTypeSwitch->ShowNextText(CurrentCharacterType);
}

void ULobbySelectionWidget::PreviousMode()
{
	if(!UISubsystem.IsValid() || GameModeSwitch == nullptr)
	{
		return;
	}
	FText CurrentMode = GameModeSwitch->GetCurrentlyDisplayedText();
	CurrentMode = UISubsystem->GetPreviousServerMode(CurrentMode, true);
	GameModeSwitch->ShowPreviousText(CurrentMode);
}

void ULobbySelectionWidget::NextMode()
{
	if(!UISubsystem.IsValid() || GameModeSwitch == nullptr)
	{
		return;
	}
	FText CurrentMode = GameModeSwitch->GetCurrentlyDisplayedText();
	CurrentMode = UISubsystem->GetNextServerMode(CurrentMode, true);
	GameModeSwitch->ShowNextText(CurrentMode);
}

void ULobbySelectionWidget::OnListEntrySelected(UServerListItemWidget* SelectedEntry)
{
	if(SelectedServerListEntry.IsValid())
	{
		SelectedServerListEntry->DeselectEntry();
	}
	if(SelectedEntry != nullptr)
	{
		SelectedServerListEntry = SelectedEntry;
		SelectedServerListEntry->SelectEntry();
	}
	
}


void ULobbySelectionWidget::AddItemToServerList(TObjectPtr<UServerListItemWidget> NewServer)
{
	if(ServerList == nullptr || NewServer == nullptr)
	{
		return;
	}
	FOnEntrySelectedDelegate* OnEntrySelectedDelegate = NewServer->GetEntrySelectedDelegate();
	if(OnEntrySelectedDelegate != nullptr)
	{
		OnEntrySelectedDelegate->BindUObject(this, &ThisClass::OnListEntrySelected);
	}
	ServerList->AddChild(NewServer);
}

void ULobbySelectionWidget::ClearServerList() 
{
	if(ServerList != nullptr)
	{
		if(SelectedServerListEntry != nullptr)
		{
			SelectedServerListEntry->DeselectEntry();
		}
		SelectedServerListEntry = nullptr;
		ServerList->ClearChildren();
	}
}

void ULobbySelectionWidget::OnPrivacySwitchChanged()
{
	if(UISubsystem.IsValid() && PrivacySwitch != nullptr)
	{
		UISubsystem->SetServerSearchPrivacyFilter(PrivacySwitch->GetCurrentlyDisplayedText());
	}
}

void ULobbySelectionWidget::OnRequiredCharacterTypeSwitchChanged()
{
	if(UISubsystem.IsValid() && RequiredCharacterTypeSwitch != nullptr)
	{
		UISubsystem->SetServerSearchRequiredCharacter(RequiredCharacterTypeSwitch->GetCurrentlyDisplayedText());
	}
}

void ULobbySelectionWidget::OnModeSwitchChanged()
{
	if(UISubsystem.IsValid() && GameModeSwitch != nullptr)
	{
		UISubsystem->SetServerSearchServerModeFilter(GameModeSwitch->GetCurrentlyDisplayedText());
	}
}

void ULobbySelectionWidget::OnAllowedLevelSpinValueChanged(float NewValue)
{
	if(UISubsystem.IsValid())
	{
		UISubsystem->SetServerSearchAllowedLevelFilter(NewValue);
	}
}

void ULobbySelectionWidget::OnVoiceChatBoxChanged(bool bIsChecked)
{
	if(UISubsystem.IsValid())
	{
		UISubsystem->SetServerSearchVoiceChatFilter(bIsChecked);
	}
}

void ULobbySelectionWidget::OnApplyFilterCheckBoxChanged(bool bIsChecked)
{
	if(PrivacySwitch != nullptr)
	{
		PrivacySwitch->SetIsEnabled(bIsChecked);
	}
	if(RequiredCharacterTypeSwitch != nullptr)
	{
		RequiredCharacterTypeSwitch->SetIsEnabled(bIsChecked);
	}
	if(GameModeSwitch != nullptr)
	{
		GameModeSwitch->SetIsEnabled(bIsChecked);
	}
	if(AllowedLevelSpin != nullptr)
	{
		AllowedLevelSpin->SetIsEnabled(bIsChecked);
	}
	if(VoiceChatBox != nullptr)
	{
		VoiceChatBox->SetIsEnabled(bIsChecked);
	}
	if(UISubsystem.IsValid())
	{
		UISubsystem->SetServerSearchIsResultFiltered(bIsChecked);
	}
}

void ULobbySelectionWidget::SetServerListIsRefreshing(bool bIsRefreshing)
{
	if(bIsRefreshing)
	{
		if(ServerListSwitcher != nullptr)
		{
			ServerListSwitcher->SetActiveWidget(ServerListLoadingWidget);
			ClearServerList();
		}
	}
	else
	{
		if(ServerListSwitcher != nullptr)
		{
			ServerListSwitcher->SetActiveWidget(ServerList);
		}
	}
}

bool ULobbySelectionWidget::IsSelectedEntryValid() const
{
	if(SelectedServerListEntry != nullptr)
	{
		return true;
	}
	return false;
}
