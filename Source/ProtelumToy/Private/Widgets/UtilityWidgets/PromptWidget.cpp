// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/UtilityWidgets/PromptWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UPromptWidget::Initialize()
{
	if(!Super::Initialize())
	{
		return false; 
	}
	if(!ensure(LeftButton != nullptr)) return false;
	LeftButton->OnClicked.AddDynamic(this, &UPromptWidget::LeftButtonCallback);
	if(!ensure(MiddleButton != nullptr)) return false;
	MiddleButton->OnClicked.AddDynamic(this, &UPromptWidget::MiddleButtonCallback);
	if(!ensure(RightButton != nullptr)) return false;
	RightButton->OnClicked.AddDynamic(this, &UPromptWidget::RightButtonCallback);
	
	OnPromptReply.AddUObject(this, &UPromptWidget::OnPromptChoiceMade);
	
	return true;
}

FOnPlayerPromptChoice* UPromptWidget::GetPromptReplyDelegate()
{
	return  &OnPromptReply;
}

void UPromptWidget::SetPromptType(EPromptType Type)
{
	PromptType = Type;
	switch (Type)
	{
		case EPromptType::OkPrompt:
			CreateOKPrompt();
			break;
		case EPromptType::OkCancelPrompt:
			CreateOKCancelPrompt();
			break;
		case EPromptType::YesNoCancelPrompt:
			CreateYesNoCancelPrompt();
			break;
		case EPromptType::YesNoPrompt:
			CreateYesNoPrompt();
			break;
	default: ;
	}
}

void UPromptWidget::SetPromptText(const FText& InDisplayText, const FText& PromptHeader) const
{
	if(DisplayText != nullptr)
	{
		DisplayText->SetText(InDisplayText);
	}
	if(HeaderText != nullptr)
	{
		HeaderText->SetText(PromptHeader);
	}
}

void UPromptWidget::OnPromptChoiceMade(EPromptReply Reply)
{
	this->RemoveFromParent();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UPromptWidget::LeftButtonCallback()
{
	EPromptReply PromptReply = EPromptReply::None;
	switch (PromptType)
	{
	case EPromptType::OkCancelPrompt:
		// ReSharper disable once CppAssignedValueIsNeverUsed
		PromptReply = EPromptReply::Ok;
		break;
		case EPromptType::YesNoCancelPrompt:
		case EPromptType::YesNoPrompt:
			// ReSharper disable once CppAssignedValueIsNeverUsed
			PromptReply = EPromptReply::Yes;
		break;
	default: ;
	}
	OnPromptReply.Broadcast(PromptReply);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UPromptWidget::MiddleButtonCallback()
{
	EPromptReply PromptReply = EPromptReply::None;
	switch (PromptType)
	{
	case EPromptType::OkPrompt:
		// ReSharper disable once CppAssignedValueIsNeverUsed
		PromptReply = EPromptReply::Ok;
		break;
	case EPromptType::YesNoCancelPrompt:
		// ReSharper disable once CppAssignedValueIsNeverUsed
		PromptReply = EPromptReply::Cancel;
		break;
	default: ;
	}
	OnPromptReply.Broadcast(PromptReply);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UPromptWidget::RightButtonCallback()
{
	EPromptReply PromptReply = EPromptReply::None;
	switch (PromptType)
	{
	case EPromptType::OkCancelPrompt:
		// ReSharper disable once CppAssignedValueIsNeverUsed
		PromptReply = EPromptReply::Cancel;
		break;
	case EPromptType::YesNoCancelPrompt:
		// ReSharper disable once CppAssignedValueIsNeverUsed
		PromptReply = EPromptReply::Cancel;
		break;
	case EPromptType::YesNoPrompt:
		// ReSharper disable once CppAssignedValueIsNeverUsed
		PromptReply = EPromptReply::No;
		break;
	default: ;
	}
	OnPromptReply.Broadcast(PromptReply);
}

void UPromptWidget::CreateOKPrompt() const
{
	if(LeftButton)
	{
		LeftButton->RemoveFromParent();
	}
	if(LeftText)
	{
		
	}
	if(MiddleButton)
	{
		
	}
	if(MiddleText)
	{
		MiddleText->SetText(OKText);
	}
	if(RightButton)
	{
		RightButton->RemoveFromParent();
	}
	if(RightText)
	{
		
	}
}

void UPromptWidget::CreateOKCancelPrompt() const
{
	if(LeftButton)
	{
		
	}
	if(LeftText)
	{
		LeftText->SetText(OKText);
	}
	if(MiddleButton)
	{
		MiddleButton->RemoveFromParent();
	}
	if(MiddleText)
	{
		
	}
	if(RightButton)
	{
		
	}
	if(RightText)
	{
		RightText->SetText(CancelText);
	}
}

void UPromptWidget::CreateYesNoCancelPrompt() const
{
	if(LeftButton)
	{
		
	}
	if(LeftText)
	{
		LeftText->SetText(YesText);
	}
	if(MiddleButton)
	{
		
	}
	if(MiddleText)
	{
		MiddleText->SetText(NoText);
	}
	if(RightButton)
	{
		
	}
	if(RightText)
	{
		RightText->SetText(CancelText);
	}
}

void UPromptWidget::CreateYesNoPrompt() const
{
	if(LeftButton)
	{
		
	}
	if(LeftText)
	{
		LeftText->SetText(YesText);
	}
	if(MiddleButton)
	{
		MiddleButton->RemoveFromParent();
	}
	if(MiddleText)
	{
		
	}
	if(RightButton)
	{
		
	}
	if(RightText)
	{
		RightText->SetText(NoText);
	}
}
