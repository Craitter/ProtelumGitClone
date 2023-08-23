// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenu/ServerListItemWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Widgets/UtilityWidgets/ServerTypes.h"

bool UServerListItemWidget::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	if(SelectButton != nullptr)
	{
		SelectButton->OnClicked.AddDynamic(this, &ThisClass::OnEntrySelected);
	}
	return true;
}

void UServerListItemWidget::SetServerListItem(const FServerListEntryData& ServerListItemData)
{
	if(ServerName != nullptr)
	{
		ServerName->SetText(ServerListItemData.ServerName);
	}

	if(HostName != nullptr)
	{
		HostName->SetText(ServerListItemData.HostName);
	}
	
	if(PasswordLockedImage != nullptr)
	{
		PasswordLockedImage->SetBrushFromTexture(ServerListItemData.PasswordLockedImage.Get());
	}
	
	if(RequiredType != nullptr)
	{
		RequiredType->SetText(ServerListItemData.RequiredType);
	}
	if(Mode != nullptr)
	{
		Mode->SetText(ServerListItemData.ServerMode);
	}
	if(LevelRange != nullptr)
	{
		LevelRange->SetText(ServerListItemData.LevelRange);
	}
	if(VoiceChat != nullptr)
	{
		VoiceChat->SetBrushFromTexture(ServerListItemData.VoiceChatImage.Get());
	}
	ServerIndex = ServerListItemData.ServerIndex;
}

FOnEntrySelectedDelegate* UServerListItemWidget::GetEntrySelectedDelegate()
{
	return &OnEntrySelectedDelegate;
}

void UServerListItemWidget::SelectEntry()
{
	if(SelectButton != nullptr)
	{
		SelectButton->SetBackgroundColor(FLinearColor::Red);
	}
	//Todo: Change Visual Appearance
}

void UServerListItemWidget::DeselectEntry()
{
	if(SelectButton != nullptr)
	{
		SelectButton->SetBackgroundColor(FLinearColor::Gray);
	}
	//Todo: Change Visual Appearance
}

int32 UServerListItemWidget::GetServerIndex() const
{
	return ServerIndex;
}

FText UServerListItemWidget::GetLevelRangeAsText() const
{
	if(LevelRange != nullptr)
	{
		return LevelRange->GetText();
	}
	return FText::GetEmpty();
}

FText UServerListItemWidget::GetCharacterTypeAsText() const
{
	if(RequiredType != nullptr)
	{
		return RequiredType->GetText();
	}
	return FText::GetEmpty();
}

void UServerListItemWidget::OnEntrySelected()
{
	if(OnEntrySelectedDelegate.ExecuteIfBound(this))
	{
		return;
	}
}

