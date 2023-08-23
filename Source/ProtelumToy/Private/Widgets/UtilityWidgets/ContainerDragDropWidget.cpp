// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\Public\Widgets\UtilityWidgets\ContainerDragDropWidget.h"

#include "Components/Image.h"

void UContainerDragDropWidget::SetDragDropImage(TWeakObjectPtr<UTexture2D> NewImage) const
{
	if(NewImage != nullptr && ItemImage != nullptr)
	{
		ItemImage->SetBrushFromTexture(NewImage.Get());
	}
}
