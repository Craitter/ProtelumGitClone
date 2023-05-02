// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProtelumGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UProtelumGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UFUNCTION(exec)
	void Host() const;

	UFUNCTION(exec)
	void Join(const FString& AddressToJoin) const;
};
