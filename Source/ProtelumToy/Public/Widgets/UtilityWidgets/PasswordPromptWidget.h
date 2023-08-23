// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessageTypes.h"
#include "Widgets/ProtelumWidget.h"
#include "PasswordPromptWidget.generated.h"

class UEditableText;
class UUISubsystem;
enum class EPromptReply;
class UButton;
class UTextBlock;
class UEditableTextBox;
/**
 * 
 */

UCLASS()
class PROTELUMTOY_API UPasswordPromptWidget : public UProtelumWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	FOnPasswordEnteringConfirmedDelegate* GetOnPasswordEnteringConfirmedDelegate();

	void ShowWrongPasswordText();
private:
	FOnPasswordEnteringConfirmedDelegate OnPasswordEnteringConfirmedDelegate;
	
	UFUNCTION()
	void LeftButtonCallback();
	UFUNCTION()
	void RightButtonCallback();

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> WrongPasswordAnim = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> PasswordBox = {nullptr};
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayText = {nullptr};
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HeaderText = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LeftButton = {nullptr};
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LeftText = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RightButton = {nullptr};
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RightText = {nullptr};

	TWeakObjectPtr<UUISubsystem> UISubsystem = {nullptr};
};
