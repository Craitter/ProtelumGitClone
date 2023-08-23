// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenu/InformationPanelWidget.h"

#include "Components/Button.h"

TWeakObjectPtr<UButton> UInformationPanelWidget::GetInformationButton() const
{
		return InformationPanelButton;
}
