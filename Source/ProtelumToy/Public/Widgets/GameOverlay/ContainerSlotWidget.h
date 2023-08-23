// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ProtelumWidget.h"
#include "ContainerSlotWidget.generated.h"

class UContainerComponent;
class UUISubsystem;
struct FContainerSlot;
class UButton;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UContainerSlotWidget : public UProtelumWidget
{
	GENERATED_BODY()

public:
	
	virtual bool Initialize() override;
	void SetItemSlot(FContainerSlot& ContainerSlot, TWeakObjectPtr<UContainerComponent> InParentContainer, int32 SlotIndex);
	TWeakObjectPtr<UContainerComponent> GetParentContainer() const;
	FContainerSlot* GetSlotContent() const;
	int32 GetReferencedSlotIndex() const;

	
protected:
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
private:
	TWeakObjectPtr<UUISubsystem> UISubsystem = {nullptr};

	TWeakObjectPtr<UContainerComponent> ParentContainer = {nullptr};
	
	FContainerSlot* ReferencedSlot = {nullptr};

	int32 ReferencesSlotIndex = -1;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UButton> ItemButton = {nullptr};
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UTextBlock> StackCount = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage = {nullptr};
};
