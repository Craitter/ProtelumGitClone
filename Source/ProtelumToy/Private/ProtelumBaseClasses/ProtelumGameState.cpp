// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtelumBaseClasses/ProtelumGameState.h"

// #include "Net/UnrealNetwork.h"
#include "ProtelumBaseClasses/HideoutGameMode.h"
#include "ProtelumBaseClasses/MainMenuGameMode.h"
#include "ProtelumBaseClasses/MissionGameMode.h"
#include "ProtelumBaseClasses/ProtelumToyGameModeBase.h"
#include "Subsystems/UISubsystem.h"



void AProtelumGameState::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );
}

void AProtelumGameState::ReceivedGameModeClass()
{
	Super::ReceivedGameModeClass();

	const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if(GameInstance.IsValid())
	{
		const TWeakObjectPtr<UUISubsystem> UISubsystem = GameInstance->GetSubsystem<UUISubsystem>();
		if(UISubsystem.IsValid())
		{
			//Superclass of those is AProtelumGameModeBase
			if(GameModeClass->IsChildOf(AHideoutGameMode::StaticClass()))
			{
				UISubsystem->LoadInterfaceBaseWidget(EBaseWidgetType::HideoutOverlay);
			}
			else if(GameModeClass->IsChildOf(AMissionGameMode::StaticClass()))
			{
				UISubsystem->LoadInterfaceBaseWidget(EBaseWidgetType::MissionOverlay);
			}
			else if(GameModeClass->IsChildOf(AMainMenuGameMode::StaticClass()))
			{
				UISubsystem->LoadInterfaceBaseWidget(EBaseWidgetType::MainMenu);
			}
		}
	}
}
