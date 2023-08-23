// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtelumToyGameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROTELUMTOY_API AMainMenuGameMode : public AProtelumToyGameModeBase
{
	GENERATED_BODY()
public:
	AMainMenuGameMode();
protected:
	virtual void BeginPlay() override;

private:
};
