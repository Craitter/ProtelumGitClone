// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


UENUM()
enum class EPromptType
{
	//InformationalPrompt
	OkPrompt,
	//Stops an action or continues
	OkCancelPrompt,
	//Yes Continues, No Continues but ignores the warning, cancel returns
	YesNoCancelPrompt,
	//Confirm or cancel an action
	YesNoPrompt,
};

UENUM()
enum class EPromptReply
{
	//Accept it
	Ok,
	//does nothing then closing
	Cancel,
	// Boom it happens for real
	Yes,
	//no
	No,

	None,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerPromptChoice, EPromptReply);

DECLARE_DELEGATE_RetVal_TwoParams(bool, FOnPasswordEnteringConfirmedDelegate, EPromptReply, const FString&)

UENUM()
enum class EPromptMessageType
{
	QuitGame,

	MainMenu,

	DeleteCharacter,

	NoSymbols,

	DuplicatedName,

	MaxCharacterCount,

	InvalidNameSize,

	NoServerSelected,

	ServerIsNotValid,

	CharacterNotMatchingRequirements,
	
};