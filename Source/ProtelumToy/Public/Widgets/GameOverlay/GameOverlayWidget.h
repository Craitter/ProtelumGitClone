// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ProtelumUserInterfaceBaseWidget.h"
#include "GameOverlayWidget.generated.h"

class UCanvasPanelSlot;
class UCanvasPanel;
class UPauseMenuWidget;
class UExpandedInventoryWidget;
class UInventoryWidget;
class UGameHUDWidget;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROTELUMTOY_API UGameOverlayWidget : public UProtelumUserInterfaceBaseWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	//Todo: maybe event based
	void StartInteractionTimer(float TimeToHold) const;
	void StopInteractionTimer() const;
	void ShowInteractionPrompt(const FText& NotifyText, float TimeToHold) const;
	void HideInteractionPrompt() const;

	void ShowGameHUD();
	
	void ShowInventory(TWeakObjectPtr<UUserWidget> InventoryContainer);

	void ShowInventoryExpanded();

	void ShowPauseMenu();

	void SetDragWidget(TObjectPtr<UUserWidget> InItemToDrag);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
private:

	TWeakObjectPtr<UUISubsystem> UISubsystem = {nullptr};
	
	TWeakObjectPtr<APlayerController> PlayerController = {nullptr};
	
	UPROPERTY()
	TObjectPtr<UUserWidget> DraggedItem = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGameHUDWidget> GameHUD = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryWidget> Inventory = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UExpandedInventoryWidget> InventoryExpanded = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPauseMenuWidget> PauseMenu = {nullptr};
};
