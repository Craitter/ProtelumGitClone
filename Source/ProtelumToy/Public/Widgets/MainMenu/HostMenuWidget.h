// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ProtelumWidget.h"
#include "HostMenuWidget.generated.h"

class UUISubsystem;
class UCharacterSelectionWidget;
class UWidgetSwitcher;
class UButton;
class UHostSettingsWidget;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UHostMenuWidget : public UProtelumWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	UFUNCTION()
	void OpenHostSettings();
	UFUNCTION()
	void OpenCharacterSelection();

protected:

	UFUNCTION()
	void Host();
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHostSettingsWidget> HostSettings = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCharacterSelectionWidget> CharacterSelection = {nullptr};

	TWeakObjectPtr<UUISubsystem> UISubsystem = {nullptr};
};
