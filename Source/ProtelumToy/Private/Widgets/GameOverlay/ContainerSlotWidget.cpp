// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameOverlay/ContainerSlotWidget.h"

#include "ActorComponents/ContainerComponent.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Subsystems/UISubsystem.h"
#include "Widgets/UtilityWidgets/ContainerDragDropWidget.h"


bool UContainerSlotWidget::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	if(ItemButton != nullptr)
	{
		
		// ItemButton->OnClicked.AddDynamic()
		// ItemButton->OnPressed.AddDynamic()
		// ItemButton->OnReleased.AddDynamic()
	}
	const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if(GameInstance.IsValid())
	{
		UISubsystem = GameInstance->GetSubsystem<UUISubsystem>();
	}

	return true;
}

void UContainerSlotWidget::SetItemSlot(FContainerSlot& ContainerSlot, TWeakObjectPtr<UContainerComponent> InParentContainer, int32 SlotIndex)
{
	if(ParentContainer == nullptr)
	{
		return;
	}
	const FCollectableItem* Item = ParentContainer->GetItemReferenceByTag(ContainerSlot.ItemTag);
	ReferencesSlotIndex = SlotIndex;
	ParentContainer = InParentContainer;
	ReferencedSlot = &ContainerSlot;
	if(ItemButton == nullptr || ItemImage == nullptr || StackCount == nullptr || Item == nullptr)
	{
		UE_LOG(LogTemp, Error , TEXT("%s() Something was null but it should not"), *FString(__FUNCTION__));
		return;
	}
	
	if(ContainerSlot.StackSize <= 0 || ContainerSlot.ItemTag == FGameplayTag::EmptyTag)
	{
		StackCount->SetVisibility(ESlateVisibility::Hidden);
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	ItemImage->SetBrushFromTexture(Item->Thumbnail);
	StackCount->SetText(FText::AsNumber(ContainerSlot.StackSize));
	SetVisibility(ESlateVisibility::Visible);
}

TWeakObjectPtr<UContainerComponent> UContainerSlotWidget::GetParentContainer() const
{
	return ParentContainer;
}

FContainerSlot* UContainerSlotWidget::GetSlotContent() const
{
	return ReferencedSlot;
}

int32 UContainerSlotWidget::GetReferencedSlotIndex() const
{
	return ReferencesSlotIndex;
}


FReply UContainerSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry,
                                                            const FPointerEvent& InMouseEvent)
{
	if(UISubsystem.IsValid())
	{
		UISubsystem->ContainerSlotClicked(this, InMouseEvent.GetEffectingButton());
	}
	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	
}
