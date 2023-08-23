// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ProtelumWidget.h"
#include "InformationPanelWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UInformationPanelWidget : public UProtelumWidget
{
	GENERATED_BODY()

public:
	TWeakObjectPtr<UButton> GetInformationButton() const;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> InformationPanelButton = {nullptr};
};
