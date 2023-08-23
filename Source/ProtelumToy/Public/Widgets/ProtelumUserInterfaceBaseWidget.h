// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ProtelumWidget.h"
#include "ProtelumUserInterfaceBaseWidget.generated.h"

class UMenuAnchor;
class UUISubsystem;
class UWidgetSwitcher;
/**
 * 
 */
UENUM()
enum class EBaseWidgetType
{
	None,
	
	MainMenu,

	MissionOverlay,

	HideoutOverlay
};

UCLASS(Abstract, NotBlueprintable, NotBlueprintType)
class PROTELUMTOY_API UProtelumUserInterfaceBaseWidget : public UProtelumWidget
{
	GENERATED_BODY()
public:

	UMenuAnchor* GetPromptSlot() const;
	void DisableSwitch() const;
	void EnableSwitch() const;
	EBaseWidgetType GetWidgetBaseType() const;

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuAnchor> PromptAnchor = {nullptr};

	TWeakObjectPtr<UUISubsystem> UISubsystem = {nullptr};

	EBaseWidgetType WidgetType;
};
