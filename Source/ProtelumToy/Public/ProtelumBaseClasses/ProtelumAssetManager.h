// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ProtelumAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UProtelumAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UProtelumAssetManager& Get();

	/** Starts initial load, gets called from InitializeObjectReferences */
	virtual void StartInitialLoading() override;
};
