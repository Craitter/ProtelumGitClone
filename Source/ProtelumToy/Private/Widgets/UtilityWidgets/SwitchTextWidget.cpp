// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/UtilityWidgets/SwitchTextWidget.h"

#include "Components/TextBlock.h"


bool USwitchTextWidget::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	OnHideTextDelegate.BindDynamic(this, &USwitchTextWidget::UpdateDisplayedTextToCurrentText);

	return true;
}

UButton* USwitchTextWidget::GetDisplayPreviousTextButton() const
{
	return DisplayPreviousTextButton;
}

FText USwitchTextWidget::GetCurrentlyDisplayedText() const
{
	if(DisplayedText != nullptr)
	{
		return DisplayedText->GetText();
	}
	return FText::GetEmpty();
}

UButton* USwitchTextWidget::GetDisplayNextTextButton() const
{
	return DisplayNextTextButton;
}

void USwitchTextWidget::ShowPreviousText(const FText& PreviousText)
{
	bIsNext = false;
	CurrentlyDisplayedText = PreviousText;
	BindToAnimationFinished(SlideAnimationPrevious, OnHideTextDelegate);
	PlayAnimationForward(SlideAnimationPrevious);
}

void USwitchTextWidget::ShowNextText(const FText& NextText)
{
	bIsNext = true;
	CurrentlyDisplayedText = NextText;
	BindToAnimationFinished(SlideAnimationNext, OnHideTextDelegate);
	PlayAnimationForward(SlideAnimationNext);
}

void USwitchTextWidget::ForceDisplayedText(const FText& ForcedText)
{
	bIsForced = true;
	CurrentlyDisplayedText = ForcedText;
	UpdateDisplayedTextToCurrentText();
}

void USwitchTextWidget::UpdateDisplayedTextToCurrentText()
{
	if(DisplayedText != nullptr)
	{
		DisplayedText->SetText(CurrentlyDisplayedText);
		OnDisplayedTextChanged.Broadcast();
	}
	if(bIsForced)
	{
		bIsForced = false;
		return;
	}
	UnbindFromAnimationFinished(SlideAnimationPrevious, OnHideTextDelegate);
	UnbindFromAnimationFinished(SlideAnimationNext, OnHideTextDelegate);
	if(bIsNext)
	{
		PlayAnimation(SlideAnimationPrevious, 0, 1, EUMGSequencePlayMode::Reverse);
	}
	else
	{
		PlayAnimation(SlideAnimationNext, 0, 1, EUMGSequencePlayMode::Reverse);
	}
}
