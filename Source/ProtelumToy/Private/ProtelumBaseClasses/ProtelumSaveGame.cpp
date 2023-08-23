// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtelumBaseClasses/ProtelumSaveGame.h"

UProtelumSaveGame::UProtelumSaveGame()
{
	UserIndex = 0;

	CharacterType = FGameplayTag::EmptyTag;
	Level = 1;
	bLastPlayed = true;
	
	TimePlayed = 0.0f;
	
}
