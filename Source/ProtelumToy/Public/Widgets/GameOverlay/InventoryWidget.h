// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ProtelumWidget.h"
#include "InventoryWidget.generated.h"

class UMenuAnchor;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UInventoryWidget : public UProtelumWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UMenuAnchor* GetInventoryAnchor();
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuAnchor> InventoryAnchor = {nullptr};
};
