// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ProtelumWidget.h"
#include "Widgets/UtilityWidgets/MessageTypes.h"
#include "CharacterSelectionWidget.generated.h"

class UTextBlock;
class UEditableText;
class USwitchTextWidget;
class UWidgetSwitcher;
class UUISubsystem;
class UButton;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UCharacterSelectionWidget : public UProtelumWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UButton* GetContinueButton() const;

	UTextBlock* GetContinueText() const;
	
	void OnPromptChoiceMade(EPromptReply PromptReply);

	void DetermineFirstCharacterSelectionScreen();

	void SetServerRequirements(const FText& RequiredCharacterType, const FText& RequiredLevelRange);
protected:

	virtual void NativePreConstruct() override;
	
	UFUNCTION()
	void OpenCharacterSelection();
	UFUNCTION()
	void OpenCharacterCreation();
	UFUNCTION()
	void OpenDeleteCharacterPrompt();
	UFUNCTION()
	void CreateCharacter();
	UFUNCTION()
	void OnCharacterNameSwitchPrevious();
	UFUNCTION()
	void OnCharacterNameSwitchNext();
	UFUNCTION()
	void OnCharacterTypeSwitchPrevious();
	UFUNCTION()
	void OnCharacterTypeSwitchNext();
	UFUNCTION()
	void UpdateCharacterInfo() const;
	UFUNCTION()
	void UpdateCharacterTypeDescription() const;
private:
	static void UpdateTextBlock(TWeakObjectPtr<UTextBlock> TextBlock, const FText& NewText);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> CharacterSelection = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> CharacterCreation = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USwitchTextWidget> CharacterNameSwitch = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USwitchTextWidget> CharacterTypeSwitch = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> CharacterNameEntry = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ContinueText = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ContinueButton = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CreateCharacterButton = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DeleteCharacterButton = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CancelCreateButton = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmCreateButton = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CharacterType = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CharacterTypeDescription = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CharacterLevel = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CharacterArtifactCount = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CharacterTimePlayed = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CharacterLastPlayedWith = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ServerRequirements = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RequiredType = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RequiredLevel = {nullptr};
	
	TWeakObjectPtr<UUISubsystem> UISubsystem = {nullptr};
	
	FString CurrentCharacterName;
};
