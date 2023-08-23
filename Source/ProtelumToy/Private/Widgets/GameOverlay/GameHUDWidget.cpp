// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameOverlay/GameHUDWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

bool UGameHUDWidget::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	if(InteractionNotifyPrompt != nullptr)
	{
		InteractionNotifyPrompt->SetVisibility(ESlateVisibility::Hidden);
	}
	if(InteractionTimeBar != nullptr)
	{
		InteractionTimeBar->SetVisibility(ESlateVisibility::Hidden);
	}
	return true;
}

void UGameHUDWidget::StartInteractionTimer(float TimeToHold)
{
	//Todo: make timer and visibility work properly
}

void UGameHUDWidget::StopInteractionTimer()
{
}

void UGameHUDWidget::ShowInteractionPrompt(const FText& NotifyText, float TimeToHold) const
{
	if(InteractionNotifyPrompt != nullptr)
	{
		InteractionNotifyPrompt->SetText(NotifyText);
		InteractionNotifyPrompt->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if(TimeToHold > 0 && InteractionTimeBar != nullptr)
	{
		InteractionTimeBar->SetPercent(0.0f);
		InteractionTimeBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGameHUDWidget::HideInteractionPrompt() const
{
	if(InteractionNotifyPrompt != nullptr)
	{
		InteractionNotifyPrompt->SetText(FText::GetEmpty());
		InteractionNotifyPrompt->SetVisibility(ESlateVisibility::Hidden);
	}
	if(InteractionTimeBar != nullptr)
	{
		InteractionTimeBar->SetVisibility(ESlateVisibility::Hidden);
	}
}
