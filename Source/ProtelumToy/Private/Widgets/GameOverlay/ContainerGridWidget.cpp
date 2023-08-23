// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameOverlay/ContainerGridWidget.h"

#include "ActorComponents/ContainerComponent.h"
#include "Components/WrapBox.h"
#include "Widgets/GameOverlay/ContainerSlotWidget.h"

UContainerGridWidget::UContainerGridWidget()
{
	const ConstructorHelpers::FClassFinder<UUserWidget> SlotBP(TEXT("/Game/WidgetSystem/Utility/Inventory/WBP_ContainerSlot"));
	if(!ensure(SlotBP.Class != nullptr)) return;
	SlotClass = SlotBP.Class;

}

bool UContainerGridWidget::Initialize()
{
	return Super::Initialize();
}

void UContainerGridWidget::ShowContainerContent(TWeakObjectPtr<UContainerComponent> InParentContainer) const
{
	if(!InParentContainer.IsValid())
	{
		return;
	}
	FContainerType& Container = InParentContainer->GetItemContainer();
	if(ContainerGrid != nullptr)
	{
		ContainerGrid->ClearChildren();
	}
	for(int32 i = 0; i < Container.ContainerContent.Num(); i++)
	{
		FContainerSlot& ContainerSlot = Container.ContainerContent[i];
		
		TObjectPtr<UContainerSlotWidget> NewSlot = CreateWidget<UContainerSlotWidget>(GetGameInstance(), SlotClass);
		if(NewSlot != nullptr)
		{
			NewSlot->SetItemSlot(ContainerSlot, InParentContainer, i);
			ContainerGrid->AddChildToWrapBox(NewSlot);
		}
	}
}
