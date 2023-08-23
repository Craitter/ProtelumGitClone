// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ProtelumGameState.generated.h"

enum class EBaseWidgetType;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API AProtelumGameState : public AGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;
	virtual void ReceivedGameModeClass() override;


private:
};
