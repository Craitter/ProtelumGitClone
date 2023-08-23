// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ProtelumWidget.h"
#include "SwitchTextWidget.generated.h"

class UTextBlock;
class UButton;

DECLARE_MULTICAST_DELEGATE(FOnDisplayedTextChanged)
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API USwitchTextWidget : public UProtelumWidget
{
	GENERATED_BODY()
public:

	virtual bool Initialize() override;
	
	UButton* GetDisplayPreviousTextButton() const;

	FText GetCurrentlyDisplayedText() const;
	
	UButton* GetDisplayNextTextButton() const;

	void ShowPreviousText(const FText& PreviousEntryText);

	void ShowNextText(const FText& NextEntryText);

	void ForceDisplayedText(const FText& ForcedEntryText);

	FWidgetAnimationDynamicEvent OnHideTextDelegate;

	UFUNCTION()
	void UpdateDisplayedTextToCurrentText();

	FOnDisplayedTextChanged OnDisplayedTextChanged;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DisplayPreviousTextButton = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayedText = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DisplayNextTextButton = {nullptr};

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SlideAnimationNext = {nullptr};

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SlideAnimationPrevious = {nullptr};

	FText CurrentlyDisplayedText;

	bool bIsNext = false;
	bool bIsForced = false;
};
