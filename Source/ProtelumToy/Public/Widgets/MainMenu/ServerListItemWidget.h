// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ProtelumWidget.h"
#include "ServerListItemWidget.generated.h"

class UButton;
struct FServerListEntryData;
class UImage;
class UTextBlock;
class UServerListItemWidget;
/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnEntrySelectedDelegate, UServerListItemWidget*)

UCLASS()
class PROTELUMTOY_API UServerListItemWidget : public UProtelumWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	
	void SetServerListItem(const FServerListEntryData& ServerListItemData);

	FOnEntrySelectedDelegate* GetEntrySelectedDelegate();

	void SelectEntry();

	void DeselectEntry();

	int32 GetServerIndex() const;

	FText GetLevelRangeAsText() const;
	FText GetCharacterTypeAsText() const;
private:
	UFUNCTION()
	void OnEntrySelected();
	
	FOnEntrySelectedDelegate OnEntrySelectedDelegate;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectButton = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ServerName = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HostName = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PasswordLockedImage = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RequiredType = {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Mode = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelRange = {nullptr};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> VoiceChat = {nullptr};

	int32 ServerIndex = -1;
};
