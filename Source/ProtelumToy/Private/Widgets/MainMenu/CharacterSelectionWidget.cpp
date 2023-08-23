// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenu/CharacterSelectionWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "..\..\..\Public\Subsystems\UISubsystem.h"
#include "..\..\..\Public\Widgets\UtilityWidgets\MessageTypes.h"
#include "Widgets/UtilityWidgets/SwitchTextWidget.h"

bool UCharacterSelectionWidget::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	if(!ensure(CreateCharacterButton != nullptr)) return false;
	CreateCharacterButton->OnClicked.AddDynamic(this, &ThisClass::OpenCharacterCreation);

	if(!ensure(DeleteCharacterButton != nullptr)) return false;
	DeleteCharacterButton->OnClicked.AddDynamic(this, &ThisClass::OpenDeleteCharacterPrompt);

	if(!ensure(ConfirmCreateButton != nullptr)) return false;
	ConfirmCreateButton->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::CreateCharacter);

	if(!ensure(CancelCreateButton != nullptr)) return false;
	CancelCreateButton->OnClicked.AddDynamic(this, &ThisClass::OpenCharacterSelection);

	if(ServerRequirements != nullptr)
	{
		ServerRequirements->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(CharacterNameSwitch != nullptr)
	{
		const TWeakObjectPtr<UButton> DisplayPreviousTextButton = CharacterNameSwitch->GetDisplayPreviousTextButton();
		if(DisplayPreviousTextButton.IsValid())
		{
			DisplayPreviousTextButton->OnClicked.AddDynamic(this, &ThisClass::OnCharacterNameSwitchPrevious);
		}
		const TWeakObjectPtr<UButton> DisplayNextTextButton = CharacterNameSwitch->GetDisplayNextTextButton();
		if(DisplayNextTextButton.IsValid())
		{
			DisplayNextTextButton->OnClicked.AddDynamic(this, &ThisClass::OnCharacterNameSwitchNext);
		}
		CharacterNameSwitch->OnDisplayedTextChanged.AddUObject(this, &UCharacterSelectionWidget::UpdateCharacterInfo);
	}
	if(CharacterTypeSwitch != nullptr)
	{
		const TWeakObjectPtr<UButton> DisplayPreviousTextButton = CharacterTypeSwitch->GetDisplayPreviousTextButton();
		if(DisplayPreviousTextButton.IsValid())
		{
			DisplayPreviousTextButton->OnClicked.AddDynamic(this, &ThisClass::OnCharacterTypeSwitchPrevious);
		}
		const TWeakObjectPtr<UButton> DisplayNextTextButton = CharacterTypeSwitch->GetDisplayNextTextButton();
		if(DisplayNextTextButton.IsValid())
		{
			DisplayNextTextButton->OnClicked.AddDynamic(this, &ThisClass::OnCharacterTypeSwitchNext);
		}
		CharacterTypeSwitch->OnDisplayedTextChanged.AddUObject(this, &UCharacterSelectionWidget::UpdateCharacterTypeDescription);
	}
	const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if(GameInstance.IsValid())
	{
		UISubsystem = GameInstance->GetSubsystem<UUISubsystem>();
	}
	return true;
}

UButton* UCharacterSelectionWidget::GetContinueButton() const
{
	return ContinueButton;
}

UTextBlock* UCharacterSelectionWidget::GetContinueText() const
{
	return  ContinueText;
}

void UCharacterSelectionWidget::OnPromptChoiceMade(EPromptReply PromptReply)
{
	if(PromptReply == EPromptReply::Yes && CharacterNameSwitch != nullptr)
	{
		if(UISubsystem.IsValid())
		{
			bool bWasLastCharacter = false;
			const FText NewCharacterName = UISubsystem->DeleteCharacter(bWasLastCharacter);
			if(bWasLastCharacter)
			{
				OpenCharacterCreation();
				return;
			}
			CharacterNameSwitch->ForceDisplayedText(NewCharacterName);
		}
	}
}

void UCharacterSelectionWidget::DetermineFirstCharacterSelectionScreen()
{
	if(UISubsystem.IsValid())
	{
		const FText LastPlayedCharacter = UISubsystem->GetActiveCharacter();
		if(LastPlayedCharacter.IdenticalTo(FText::GetEmpty()))
		{
			OpenCharacterCreation();
		}
		else
		{
			OpenCharacterSelection();
			CharacterNameSwitch->ForceDisplayedText(LastPlayedCharacter);
		}

	}
}

void UCharacterSelectionWidget::SetServerRequirements(const FText& RequiredCharacterType,
	const FText& RequiredLevelRange)
{
	if(RequiredType == nullptr || RequiredLevel == nullptr || ServerRequirements == nullptr)
	{
		return;
	}
	RequiredType->SetText(RequiredCharacterType);
	RequiredLevel->SetText(RequiredLevelRange);
	ServerRequirements->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UCharacterSelectionWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}


void UCharacterSelectionWidget::OpenCharacterSelection()
{
	if(Switcher != nullptr)
	{
		Switcher->SetActiveWidget(CharacterSelection);
	}
	if(UISubsystem.IsValid())
	{
		const FText DisplayedCharacter = UISubsystem->GetActiveCharacter();
		if(DisplayedCharacter.IdenticalTo(FText::GetEmpty()))
		{
			OpenCharacterCreation();
		}
	}
}

void UCharacterSelectionWidget::OpenCharacterCreation()
{
	if(Switcher != nullptr )
	{
		Switcher->SetActiveWidget(CharacterCreation);
		
	}
	if(UISubsystem.IsValid() && CharacterTypeSwitch != nullptr)
	{
		CharacterTypeSwitch->ForceDisplayedText(UISubsystem->GetNextCharacterType(FText::GetEmpty()));
		
		const FText LastPlayedCharacter = UISubsystem->GetActiveCharacter();
		if(LastPlayedCharacter.IdenticalTo(FText::GetEmpty()))
		{
			if(CancelCreateButton != nullptr)
			{
				CancelCreateButton->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else
		{
			if(CancelCreateButton != nullptr)
			{
				CancelCreateButton->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UCharacterSelectionWidget::OpenDeleteCharacterPrompt()
{
	if(UISubsystem.IsValid())
	{
		bool bWasSuccessful;
		FOnPlayerPromptChoice* PromptChoice = UISubsystem->CreatePrompt(EPromptType::YesNoPrompt,
		                                                                EPromptMessageType::DeleteCharacter, bWasSuccessful);
		if(PromptChoice != nullptr)
		{
			PromptChoice->AddUObject(this, &UCharacterSelectionWidget::OnPromptChoiceMade);
		}
	}
}

void UCharacterSelectionWidget::CreateCharacter()
{
	if(!UISubsystem.IsValid() || CharacterNameEntry == nullptr || CharacterTypeSwitch == nullptr)
	{
		return;
	}
	const FText& NewCharacterName = CharacterNameEntry->GetText();
	const FText& TempCharacterType = CharacterTypeSwitch->GetCurrentlyDisplayedText();
	bool bWasValidEntry;
	UISubsystem->CreateCharacter(NewCharacterName, TempCharacterType, bWasValidEntry);
	if(!bWasValidEntry)
	{
		return;
	}
	CharacterNameSwitch->ForceDisplayedText(NewCharacterName);
	OpenCharacterSelection();
}

void UCharacterSelectionWidget::OnCharacterNameSwitchPrevious()
{
	if(CharacterNameSwitch != nullptr)
	{
		if(UISubsystem.IsValid())
		{
			const FText PreviousCharacter = UISubsystem->GetPreviousCharacter();
			CharacterNameSwitch->ShowPreviousText(PreviousCharacter);
		}
	}
}

void UCharacterSelectionWidget::OnCharacterNameSwitchNext()
{
	if(CharacterNameSwitch != nullptr)
	{
		if(UISubsystem.IsValid())
		{
			const FText NextCharacter = UISubsystem->GetNextCharacter();
			CharacterNameSwitch->ShowNextText(NextCharacter);
		}
	}
}

void UCharacterSelectionWidget::OnCharacterTypeSwitchPrevious()
{
	if(CharacterTypeSwitch == nullptr || !UISubsystem.IsValid())
	{
		return;
	}
	FText CurrentType = CharacterTypeSwitch->GetCurrentlyDisplayedText();
	CurrentType = UISubsystem->GetPreviousCharacterType(CurrentType);
	CharacterTypeSwitch->ShowPreviousText(CurrentType);
}

void UCharacterSelectionWidget::OnCharacterTypeSwitchNext()
{
	if(CharacterTypeSwitch == nullptr || !UISubsystem.IsValid())
	{
		return;
	}
	FText CurrentType = CharacterTypeSwitch->GetCurrentlyDisplayedText();
	CurrentType = UISubsystem->GetNextCharacterType(CurrentType);
	CharacterTypeSwitch->ShowNextText(CurrentType);
}

void UCharacterSelectionWidget::UpdateCharacterInfo() const
{
	if(!UISubsystem.IsValid())
	{
		return;
	}
	UpdateTextBlock(CharacterType, UISubsystem->GetCharacterType());
	UpdateTextBlock(CharacterLevel, UISubsystem->GetCharacterLevel());
	UpdateTextBlock(CharacterArtifactCount, UISubsystem->GetCharacterCraftedArtifactsCount());
	UpdateTextBlock(CharacterTimePlayed, UISubsystem->GetCharacterPlayTimeInHours());
	UpdateTextBlock(CharacterLastPlayedWith, UISubsystem->GetCharacterLastPlayedWith());
	
}

void UCharacterSelectionWidget::UpdateCharacterTypeDescription() const
{
	if(!UISubsystem.IsValid() || CharacterTypeSwitch == nullptr)
	{
		return;
	}
	const FText Description = UISubsystem->GetCharacterTypeDescription(CharacterTypeSwitch->GetCurrentlyDisplayedText());
	UpdateTextBlock(CharacterTypeDescription, Description);
}

void UCharacterSelectionWidget::UpdateTextBlock(TWeakObjectPtr<UTextBlock> TextBlock, const FText& NewText)
{
	if(TextBlock.IsValid())
	{
		TextBlock->SetText(NewText);
	}
}
