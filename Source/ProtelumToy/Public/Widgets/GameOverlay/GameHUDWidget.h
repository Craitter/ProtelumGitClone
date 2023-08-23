// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ProtelumWidget.h"
#include "GameHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UGameHUDWidget : public UProtelumWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void StartInteractionTimer(float TimeToHold);
	void StopInteractionTimer();
	void ShowInteractionPrompt(const FText& NotifyText, float TimeToHold) const;
	void HideInteractionPrompt() const;
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractionNotifyPrompt = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> InteractionTimeBar = {nullptr};
};
