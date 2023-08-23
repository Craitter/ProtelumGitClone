// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessageTypes.h"
#include "Widgets/ProtelumWidget.h"
#include "PromptWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */


UCLASS()
class PROTELUMTOY_API UPromptWidget : public UProtelumWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	
	FOnPlayerPromptChoice* GetPromptReplyDelegate();

	void SetPromptType(EPromptType Type);
	void SetPromptText(const FText& InDisplayText, const FText& PromptHeader) const;

private:
	FOnPlayerPromptChoice OnPromptReply;	

	EPromptType PromptType;
	UFUNCTION()
	void OnPromptChoiceMade(EPromptReply Reply);
	UFUNCTION()
	void LeftButtonCallback();
	UFUNCTION()
	void MiddleButtonCallback();
	UFUNCTION()
	void RightButtonCallback();

	void CreateOKPrompt() const;
	void CreateOKCancelPrompt() const;
	void CreateYesNoCancelPrompt() const;
	void CreateYesNoPrompt() const;
	
	UPROPERTY(EditDefaultsOnly)
	FText OKText;

	UPROPERTY(EditDefaultsOnly)
	FText YesText;
	
	UPROPERTY(EditDefaultsOnly)
	FText NoText;

	UPROPERTY(EditDefaultsOnly)
	FText CancelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HeaderText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LeftButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LeftText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MiddleButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MiddleText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RightButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RightText;
	
};
