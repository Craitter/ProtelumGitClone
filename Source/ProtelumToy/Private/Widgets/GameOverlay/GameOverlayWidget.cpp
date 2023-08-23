// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameOverlay/GameOverlayWidget.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/MenuAnchor.h"
#include "Components/NativeWidgetHost.h"
#include "Components/WidgetSwitcher.h"
#include "Subsystems/UISubsystem.h"
#include "Widgets/GameOverlay/GameHUDWidget.h"
#include "Widgets/GameOverlay/InventoryWidget.h"


bool UGameOverlayWidget::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if(GameInstance.IsValid())
	{
		UISubsystem = GameInstance->GetSubsystem<UUISubsystem>();
	}
	
	WidgetType = EBaseWidgetType::MissionOverlay;
	return true;
}

void UGameOverlayWidget::StartInteractionTimer(float TimeToHold) const
{
	if(GameHUD != nullptr)
	{
		GameHUD->StartInteractionTimer(TimeToHold);
	}
}

void UGameOverlayWidget::StopInteractionTimer() const
{
	if(GameHUD != nullptr)
	{
		GameHUD->StopInteractionTimer();
	}
}

void UGameOverlayWidget::ShowInteractionPrompt(const FText& NotifyText, float TimeToHold) const
{
	if(GameHUD != nullptr)
	{
		GameHUD->ShowInteractionPrompt(NotifyText, TimeToHold);
	}
}

void UGameOverlayWidget::HideInteractionPrompt() const
{
	if(GameHUD != nullptr)
	{
		GameHUD->HideInteractionPrompt();
	}
}

void UGameOverlayWidget::ShowGameHUD()
{
}

void UGameOverlayWidget::ShowInventory(TWeakObjectPtr<UUserWidget> InventoryContainer)
{
	if(Inventory != nullptr && Switcher != nullptr)
	{
		const TWeakObjectPtr<UMenuAnchor> InventoryAnchor = Inventory->GetInventoryAnchor();
		if(InventoryAnchor.IsValid())
		{
			InventoryAnchor->SetContent(InventoryContainer.Get());
			InventoryAnchor->Open(true);
		}
		Switcher->SetActiveWidget(Inventory);
	}
}

void UGameOverlayWidget::ShowInventoryExpanded()
{
}

void UGameOverlayWidget::ShowPauseMenu()
{
}

void UGameOverlayWidget::SetDragWidget(TObjectPtr<UUserWidget> InItemToDrag)
{
	if(DraggedItem != nullptr)
	{
		DraggedItem->RemoveFromParent();
	}
	DraggedItem = InItemToDrag;
	const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if(GameInstance.IsValid())
	{
		PlayerController = GameInstance->GetPrimaryPlayerController();
	}
	
}

void UGameOverlayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(DraggedItem != nullptr && PlayerController != nullptr && UISubsystem.IsValid())
	{
		float PosX;
		float PosY;
		//EDragPivot to change the Pivot (See it for reference)
		PlayerController->GetMousePosition(PosX, PosY);
		const FVector2D MousePosition = {PosX, PosY};

		const FVector2D ViewportSize = UISubsystem->GetViewportSize();
		const FVector2D ScreenResolution = UISubsystem->GetGameResolution();
		const FVector2D DesiredWidgetSize = DraggedItem->GetDesiredSize();
		const FVector2D WidgetSizeMultiplication = ViewportSize / ScreenResolution;
		const FVector2D AdjustedWidgetSize = DesiredWidgetSize * WidgetSizeMultiplication;
		const float SquareWidgetScale = FMath::Min(AdjustedWidgetSize.X, AdjustedWidgetSize.Y);
		const float CenterOfWidget = SquareWidgetScale / 2;
		const FVector2D CenterVector = {CenterOfWidget, CenterOfWidget};
		const FVector2D NewWidgetPosition = MousePosition - CenterVector;
			
		DraggedItem->SetPositionInViewport(NewWidgetPosition);
	}
}
