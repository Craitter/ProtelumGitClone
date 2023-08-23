// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ProtelumWidget.h"
#include "HostSettingsWidget.generated.h"

class UUISubsystem;
class UEditableText;
class USwitchTextWidget;
class USpinBox;
class UCheckBox;
class UButton;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UHostSettingsWidget : public UProtelumWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;
		
	UButton* GetCancelButton() const;

	UButton* GetHostButton() const;

	FText GetLobbyName() const;

	FText GetPassword() const;

	FText GetLobbyVisibility() const;

	FText GetMode() const;

	int32 GetMinLevel() const;

	int32 GetMaxLevel() const;

	bool ShouldUseVoiceChat() const;

	UFUNCTION()
	void OnMinLevelSpinChanged(float NewValue);

	UFUNCTION()
	void OnMaxLevelSpinChanged(float NewValue);

	UFUNCTION()
	void OnNextMode();

	UFUNCTION()
	void OnPreviousMode();

	UFUNCTION()
	void OnNextLobbyVisibility();

	UFUNCTION()
	void OnPreviousLobbyVisibility();

	void SetDefaultServerNamePreview() const;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CancelButton = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> HostButton = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> LobbyName = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> Password = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USwitchTextWidget> PrivacySwitch = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USwitchTextWidget> ModeSwitch = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> MinLevelSpin = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> MaxLevelSpin = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> VoiceChatBox = {nullptr};

	TWeakObjectPtr<UUISubsystem> UISubsystem = {nullptr};
};
