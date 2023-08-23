// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenu/HostSettingsWidget.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableText.h"
#include "Components/SpinBox.h"
#include "ProtelumToy/ProtelumToy.h"
#include "Subsystems/UISubsystem.h"
#include "Widgets/UtilityWidgets/SwitchTextWidget.h"

bool UHostSettingsWidget::Initialize()
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
	
	if(MinLevelSpin != nullptr)
	{
		MinLevelSpin->SetMaxValue(CHARACTER_LEVEL_MAX);
		MinLevelSpin->SetValue(CHARACTER_LEVEL_MIN);
		MinLevelSpin->SetMinValue(CHARACTER_LEVEL_MIN);
		MinLevelSpin->OnValueChanged.AddDynamic(this , &UHostSettingsWidget::OnMinLevelSpinChanged);
	}
	if(MaxLevelSpin != nullptr)
	{
		MaxLevelSpin->SetMaxValue(CHARACTER_LEVEL_MAX);
		MaxLevelSpin->SetValue(CHARACTER_LEVEL_MAX);
		MaxLevelSpin->SetMinValue(CHARACTER_LEVEL_MIN);
		MaxLevelSpin->OnValueChanged.AddDynamic(this , &UHostSettingsWidget::OnMaxLevelSpinChanged);
	}

	if(ModeSwitch != nullptr)
	{
		const TWeakObjectPtr<UButton> DisplayPreviousTextButton = ModeSwitch->GetDisplayPreviousTextButton();
		if(DisplayPreviousTextButton.IsValid())
		{
			DisplayPreviousTextButton->OnClicked.AddDynamic(this, &ThisClass::OnPreviousMode);
		}
		const TWeakObjectPtr<UButton> DisplayNextTextButton = ModeSwitch->GetDisplayNextTextButton();
		if(DisplayNextTextButton.IsValid())
		{
			DisplayNextTextButton->OnClicked.AddDynamic(this, &ThisClass::OnNextMode);
		}
		if(UISubsystem != nullptr)
		{
			ModeSwitch->ForceDisplayedText(UISubsystem->GetNextServerMode(FText::GetEmpty()));
		}
	}
	if(PrivacySwitch != nullptr)
	{
		const TWeakObjectPtr<UButton> DisplayPreviousTextButton = PrivacySwitch->GetDisplayPreviousTextButton();
		if(DisplayPreviousTextButton.IsValid())
		{
			DisplayPreviousTextButton->OnClicked.AddDynamic(this, &ThisClass::OnPreviousLobbyVisibility);
		}
		const TWeakObjectPtr<UButton> DisplayNextTextButton = PrivacySwitch->GetDisplayNextTextButton();
		if(DisplayNextTextButton.IsValid())
		{
			DisplayNextTextButton->OnClicked.AddDynamic(this, &ThisClass::OnNextLobbyVisibility);
		}
		if(UISubsystem != nullptr)
		{
			PrivacySwitch->ForceDisplayedText(UISubsystem->GetNextServerPrivacy(FText::GetEmpty(), true));
		}
	}
	


	return true;
}

UButton* UHostSettingsWidget::GetCancelButton() const
{
	return CancelButton;
}

UButton* UHostSettingsWidget::GetHostButton() const
{
	return HostButton;
}

FText UHostSettingsWidget::GetLobbyName() const
{
	if(LobbyName != nullptr)
	{
		return LobbyName->GetText();
	}
	return FText::GetEmpty();
}

FText UHostSettingsWidget::GetPassword() const
{
	if(Password != nullptr)
	{
		return Password->GetText();
	}
	return FText::GetEmpty();
}

FText UHostSettingsWidget::GetLobbyVisibility() const
{
	if(PrivacySwitch != nullptr)
	{
		return PrivacySwitch->GetCurrentlyDisplayedText();
	}
	return FText::GetEmpty();
}

FText UHostSettingsWidget::GetMode() const
{
	if(ModeSwitch != nullptr)
	{
		return ModeSwitch->GetCurrentlyDisplayedText();
	}
	return FText::GetEmpty();
}

int32 UHostSettingsWidget::GetMinLevel() const
{
	if(MinLevelSpin != nullptr)
	{
		return MinLevelSpin->GetValue();
	}
	return CHARACTER_LEVEL_MIN;
}

int32 UHostSettingsWidget::GetMaxLevel() const
{
	if(MaxLevelSpin != nullptr)
	{
		return MaxLevelSpin->GetValue();
	}
	return CHARACTER_LEVEL_MAX;
}

bool UHostSettingsWidget::ShouldUseVoiceChat() const
{
	if(VoiceChatBox != nullptr)
	{
		const ECheckBoxState State = VoiceChatBox->GetCheckedState();
		if(State == ECheckBoxState::Checked)
		{
			return true;
		}
	}
	return false;
}

void UHostSettingsWidget::OnMinLevelSpinChanged(float NewValue)
{
	if(MinLevelSpin == nullptr || MaxLevelSpin == nullptr)
	{
		return;
	}
	const int32 CurrentMin = MinLevelSpin->GetValue();
	const int32 CurrentMax = MaxLevelSpin->GetValue();
	if(CurrentMin > CurrentMax)
	{
		MaxLevelSpin->SetValue(CurrentMin);
	}
}

void UHostSettingsWidget::OnMaxLevelSpinChanged(float NewValue)
{
	if(MinLevelSpin == nullptr || MaxLevelSpin == nullptr)
	{
		return;
	}
	const int32 CurrentMin = MinLevelSpin->GetValue();
	const int32 CurrentMax = MaxLevelSpin->GetValue();
	if(CurrentMin > CurrentMax)
	{
		MinLevelSpin->SetValue(CurrentMax);
	}
}

void UHostSettingsWidget::OnNextMode()
{
	if(UISubsystem == nullptr || ModeSwitch == nullptr)
	{
		return;
	}
	const FText& CurrentText = ModeSwitch->GetCurrentlyDisplayedText();
	const FText& NextText = UISubsystem->GetNextServerMode(CurrentText);
	ModeSwitch->ShowNextText(NextText);
}

void UHostSettingsWidget::OnPreviousMode()
{
	if(UISubsystem == nullptr || ModeSwitch == nullptr)
	{
		return;
	}
	const FText& CurrentText = ModeSwitch->GetCurrentlyDisplayedText();
	const FText& PreviousText = UISubsystem->GetPreviousServerMode(CurrentText);
	ModeSwitch->ShowPreviousText(PreviousText);
}

void UHostSettingsWidget::OnNextLobbyVisibility()
{
	if(UISubsystem == nullptr || PrivacySwitch == nullptr)
	{
		return;
	}
	const FText& CurrentText = PrivacySwitch->GetCurrentlyDisplayedText();
	const FText& NextText = UISubsystem->GetNextServerPrivacy(CurrentText, true);
	PrivacySwitch->ShowNextText(NextText);
}

void UHostSettingsWidget::OnPreviousLobbyVisibility()
{
	if(UISubsystem == nullptr || PrivacySwitch == nullptr)
	{
		return;
	}
	const FText& CurrentText = PrivacySwitch->GetCurrentlyDisplayedText();
	const FText& PreviousText = UISubsystem->GetPreviousServerPrivacy(CurrentText, true);
	PrivacySwitch->ShowPreviousText(PreviousText);
}

void UHostSettingsWidget::SetDefaultServerNamePreview() const
{
	if(UISubsystem.IsValid() && LobbyName != nullptr)
	{
		LobbyName->SetHintText(UISubsystem->GetDefaultServerName());
	}
}
