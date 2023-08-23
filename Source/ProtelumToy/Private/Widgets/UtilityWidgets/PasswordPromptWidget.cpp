// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/UtilityWidgets/PasswordPromptWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Subsystems/UISubsystem.h"

bool UPasswordPromptWidget::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	if(LeftButton != nullptr)
	{
		LeftButton->OnClicked.AddDynamic(this, &ThisClass::LeftButtonCallback);
	}
	if(RightButton != nullptr)
	{
		RightButton->OnClicked.AddDynamic(this, &ThisClass::RightButtonCallback);
	}
	const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if(GameInstance.IsValid())
	{
		UISubsystem = GameInstance->GetSubsystem<UUISubsystem>();
	}
	if(UISubsystem.IsValid())
	{
		UISubsystem->EnablePrompt(this);	
	}
	
	return true;
}

FOnPasswordEnteringConfirmedDelegate* UPasswordPromptWidget::GetOnPasswordEnteringConfirmedDelegate()
{
	return &OnPasswordEnteringConfirmedDelegate;
}

void UPasswordPromptWidget::ShowWrongPasswordText()
{
	StopAnimation(WrongPasswordAnim);
	PlayAnimation(WrongPasswordAnim);
}

void UPasswordPromptWidget::LeftButtonCallback()
{
	if(PasswordBox != nullptr && OnPasswordEnteringConfirmedDelegate.IsBound())
	{
		if(!OnPasswordEnteringConfirmedDelegate.Execute(EPromptReply::Ok, PasswordBox->GetText().ToString()))
		{
			ShowWrongPasswordText();
		}
	}
}

void UPasswordPromptWidget::RightButtonCallback()
{
	if(OnPasswordEnteringConfirmedDelegate.IsBound() && OnPasswordEnteringConfirmedDelegate.Execute(EPromptReply::Cancel, FString()))
	{
		
	}
}
