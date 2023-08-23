// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ContainerComponent.h"
#include "Widgets/ProtelumWidget.h"
#include "ContainerGridWidget.generated.h"

class UContainerSlotWidget;
class UWrapBox;
struct FContainerSlot;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UContainerGridWidget : public UProtelumWidget
{
	GENERATED_BODY()

public:
	UContainerGridWidget();
	
	virtual bool Initialize() override;
	
	void ShowContainerContent(TWeakObjectPtr<UContainerComponent> InParentContainer) const;
	// void UpdateContainerContent();
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> ContainerGrid = {nullptr};

	TSubclassOf<UContainerSlotWidget> SlotClass = {nullptr};
	// TArray<TObjectPtr<UContainerSlotWidget>>
};
