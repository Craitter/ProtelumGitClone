// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ProtelumUserInterfaceBaseWidget.h"

#include "Components/WidgetSwitcher.h"

UMenuAnchor* UProtelumUserInterfaceBaseWidget::GetPromptSlot() const
{
	
	return PromptAnchor;
}

void UProtelumUserInterfaceBaseWidget::DisableSwitch() const
{
	if(Switcher != nullptr)
	{
		Switcher->SetIsEnabled(false);
	}
}

void UProtelumUserInterfaceBaseWidget::EnableSwitch() const
{
	if(Switcher != nullptr)
	{
		Switcher->SetIsEnabled(true);
	}
}

EBaseWidgetType UProtelumUserInterfaceBaseWidget::GetWidgetBaseType() const
{
	return WidgetType;
}
