// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProtelumToyGameModeBase.generated.h"

enum class EBaseWidgetType;

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable, NotBlueprintType)
class PROTELUMTOY_API AProtelumToyGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	
protected:
	virtual void BeginPlay() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
};
