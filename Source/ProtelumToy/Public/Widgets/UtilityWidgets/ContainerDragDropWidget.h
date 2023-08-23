// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ProtelumWidget.h"
#include "ContainerDragDropWidget.generated.h"

class UImage;
struct FContainerSlot;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UContainerDragDropWidget : public UProtelumWidget
{
	GENERATED_BODY()
public:
	void SetDragDropImage(TWeakObjectPtr<UTexture2D> NewImage) const;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage = {nullptr};
};
