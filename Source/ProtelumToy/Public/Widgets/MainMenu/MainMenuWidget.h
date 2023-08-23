// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ProtelumUserInterfaceBaseWidget.h"
#include "MainMenuWidget.generated.h"

class UMenuAnchor;
class UInformationPanelWidget;
class UButton;
class UWidgetSwitcher;
class UStatsMenu;
class USettingsMenuWidget;
class UJoinMenuWidget;
class UHostMenuWidget;
class UStartMainMenuWidget;
class UUISubsystem;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROTELUMTOY_API UMainMenuWidget : public UProtelumUserInterfaceBaseWidget
{
	GENERATED_BODY()

public:
	
	virtual bool Initialize() override;
	UFUNCTION()
	void OpenStartMenu();
	UFUNCTION()
	void OpenHostMenu();
	UFUNCTION()
	void OpenJoinMenu();
	UFUNCTION()
	void OpenSettingsMenu();
	UFUNCTION()
	void OpenStatsMenu();




	FWidgetAnimationDynamicEvent OnSwitchAnimEnds;
	void HideActiveWidget();
	UFUNCTION()
	void RevealActiveWidget();
protected:


private:
	TWeakObjectPtr<UProtelumWidget> NextWidget = {nullptr};
	bool bIsSwitching = false;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStartMainMenuWidget> StartMenu = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHostMenuWidget> HostMenu = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UJoinMenuWidget> JoinMenu = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USettingsMenuWidget> SettingsMenu = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStatsMenu> StatsMenu = {nullptr};

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SwitchAwayAnim = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReturnButton = {nullptr};

	
};
