// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtelumBaseClasses/ProtelumPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/ProtelumAbilitySystemComponent.h"
#include "ProtelumBaseClasses/ProtelumCharacter.h"


namespace AbilityInputBindingComponent_Impl
{
	constexpr int32 InvalidInputID = 0;
	int32 IncrementingInputID = InvalidInputID;

	static int32 GetNextInputID()	
	{
		return ++IncrementingInputID;
	}
}

AProtelumPlayerController::AProtelumPlayerController()
{
	
}

void AProtelumPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// if(ProtelumAbilitySystem.IsValid())
	// {
	// 	if(ProtelumAbilitySystem->GenericLocalConfirmCallbacks.IsBound() || ProtelumAbilitySystem->GenericLocalCancelCallbacks.IsBound())
	// 	{
	// 		if(IsValid(InputSubsystem) && !bIMCConfirmCancelBound)
	// 		{
	// 			if(!ConfirmCancelIMC.IsNull())
	// 			{
	// 				InputSubsystem->AddMappingContext(ConfirmCancelIMC.LoadSynchronous(), 1);
	// 				bIMCConfirmCancelBound = true;
	// 			}
	// 		}
	// 	}
	// 	else if(bIMCConfirmCancelBound)
	// 	{
	// 		if(IsValid(InputSubsystem))
	// 		{
	// 			if(!ConfirmCancelIMC.IsNull())
	// 			{
	// 				InputSubsystem->AddMappingContext(ConfirmCancelIMC.LoadSynchronous(), 1);
	// 				bIMCConfirmCancelBound = false;
	// 			}
	// 		}
	// 	}
	// }
}

void AProtelumPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	ProtelumCharacter = Cast<AProtelumCharacter>(GetPawn());
}

void AProtelumPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAbilitySystemSetup();
}

void AProtelumPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(const TObjectPtr<ULocalPlayer> LocalPlayer = GetLocalPlayer())
	{
		InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if(IsValid(InputSubsystem))
		{
			if(!DefaultIMC.IsNull())
			{
				InputSubsystem->AddMappingContext(DefaultIMC.LoadSynchronous(), 0);
			}
		}
	}
	if(IsValid(PlayerCameraManager))
	{
		PlayerCameraManager->ViewPitchMin = -80.0f;
		PlayerCameraManager->ViewPitchMax = 55.0f;
	}
}

void AProtelumPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(IAMove, ETriggerEvent::Triggered, this, &AProtelumPlayerController::RequestMove);
	EnhancedInputComponent->BindAction(IALook, ETriggerEvent::Triggered, this, &AProtelumPlayerController::RequestLook);
	EnhancedInputComponent->BindAction(IAConfirm, ETriggerEvent::Triggered, this, &AProtelumPlayerController::RequestConfirmAbility);
	EnhancedInputComponent->BindAction(IACancel, ETriggerEvent::Triggered, this, &AProtelumPlayerController::RequestCancelAbility);

	//Begin Does Nothing Right now Todo: Determine its purpose
	ResetBindings();
	
	for (const auto& Ability : MappedAbilities)
	{
		TObjectPtr<UInputAction> InputAction = Ability.Key;

		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AProtelumPlayerController::OnAbilityInputPressed, InputAction);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AProtelumPlayerController::OnAbilityInputReleased, InputAction);
	}
	//End Does Nothing Right now
	RunAbilitySystemSetup();
}

//should always be called AFTER granting ability!!!
void AProtelumPlayerController::SetInputBinding(TObjectPtr<UInputAction> InputAction,
	FGameplayAbilitySpecHandle AbilitySpecHandle)
{
	using namespace AbilityInputBindingComponent_Impl;
	
	
	const TObjectPtr<FGameplayAbilitySpec> BindingAbility = FindAbilitySpec(AbilitySpecHandle);

	TObjectPtr<FAbilityInputBinding> AbilityInputBinding = MappedAbilities.Find(InputAction);
	if(AbilityInputBinding)
	{
		const TObjectPtr<FGameplayAbilitySpec> OldBoundAbility = FindAbilitySpec(AbilityInputBinding->BoundAbilitiesStack.Top());
		
		if(OldBoundAbility && OldBoundAbility->InputID == AbilityInputBinding->InputID)
		{
			OldBoundAbility->InputID = InvalidInputID;
		}
	}
	else
	{
		AbilityInputBinding = &MappedAbilities.Add(InputAction);
		AbilityInputBinding->InputID = GetNextInputID();
	}

	if(BindingAbility)
	{
		BindingAbility->InputID = AbilityInputBinding->InputID;
	}

	AbilityInputBinding->BoundAbilitiesStack.Push(AbilitySpecHandle);
	TryBindAbilityInput(InputAction, *AbilityInputBinding);
}

void AProtelumPlayerController::ClearInputBinding(FGameplayAbilitySpecHandle AbilitySpecHandle)
{
	using namespace AbilityInputBindingComponent_Impl;

	if (FGameplayAbilitySpec* FoundAbility = FindAbilitySpec(AbilitySpecHandle))
	{
		// Find the mapping for this ability
		auto MappedIterator = MappedAbilities.CreateIterator();
		while (MappedIterator)
		{
			if (MappedIterator.Value().InputID == FoundAbility->InputID)
			{
				break;
			}

			++MappedIterator;
		}

		if (MappedIterator)
		{
			FAbilityInputBinding& AbilityInputBinding = MappedIterator.Value();

			if (AbilityInputBinding.BoundAbilitiesStack.Remove(AbilitySpecHandle) > 0)
			{
				if (AbilityInputBinding.BoundAbilitiesStack.Num() > 0)
				{
					FGameplayAbilitySpec* StackedAbility = FindAbilitySpec(AbilityInputBinding.BoundAbilitiesStack.Top());
					if (StackedAbility && StackedAbility->InputID == 0)
					{
						StackedAbility->InputID = AbilityInputBinding.InputID;
					}
				}
				else
				{
					// NOTE: This will invalidate the `AbilityInputBinding` ref above
					RemoveEntry(MappedIterator.Key());
				}
				// DO NOT act on `AbilityInputBinding` after here (it could have been removed)


				FoundAbility->InputID = InvalidInputID;
			}
		}
	}
}

void AProtelumPlayerController::ClearAbilityBindings(TObjectPtr<UInputAction> InputAction)
{
	RemoveEntry(InputAction);
}



void AProtelumPlayerController::OnWaitConfirmInputBegin() const
{
	if(IsValid(InputSubsystem))
	{
		if(!ConfirmCancelIMC.IsNull())
		{
			InputSubsystem->AddMappingContext(ConfirmCancelIMC.LoadSynchronous(), 1);
		}
	}
}

void AProtelumPlayerController::OnWaitConfirmInputEnd() const
{
	if(IsValid(InputSubsystem))
	{
		InputSubsystem->RemoveMappingContext(ConfirmCancelIMC.Get()); //Todo: if different GA is called CancelAbility
	}
}

void AProtelumPlayerController::RunAbilitySystemSetup()
{
	//Todo: Optimize the flow because i feel in Danger with the way it is setup
	ProtelumCharacter = Cast<AProtelumCharacter>(GetPawn());
	if(IsValid(ProtelumCharacter))
	{
		ProtelumAbilitySystem = Cast<UProtelumAbilitySystemComponent>(ProtelumCharacter->GetAbilitySystemComponent());
	}
	if(ProtelumAbilitySystem.IsValid())
	{
		for(auto& InputBinding :MappedAbilities)
		{
			const int32 NewInputID = AbilityInputBindingComponent_Impl::GetNextInputID();
			InputBinding.Value.InputID = NewInputID;

			for(const FGameplayAbilitySpecHandle AbilitySpecHandle : InputBinding.Value.BoundAbilitiesStack)
			{
				TObjectPtr<FGameplayAbilitySpec> FoundAbility = ProtelumAbilitySystem->FindAbilitySpecFromHandle(AbilitySpecHandle);
				if(FoundAbility != nullptr)
				{
					FoundAbility->InputID = NewInputID;
				}
			}
		}
	}
}

void AProtelumPlayerController::TryBindAbilityInput(TObjectPtr<UInputAction> InputAction,
	FAbilityInputBinding& AbilityInputBinding)
{
	if(!IsValid(EnhancedInputComponent))
	{
		EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	}
	if(IsValid(EnhancedInputComponent))
	{
		if(AbilityInputBinding.OnPressedHandle == 0)
		{
			AbilityInputBinding.OnPressedHandle = EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AProtelumPlayerController::OnAbilityInputPressed, InputAction).GetHandle();
		}

		// Released event
		if (AbilityInputBinding.OnReleasedHandle == 0)
		{
			AbilityInputBinding.OnReleasedHandle = EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AProtelumPlayerController::OnAbilityInputReleased, InputAction).GetHandle();
		}
	}
}

void AProtelumPlayerController::OnAbilityInputPressed(TObjectPtr<UInputAction> InputAction)
{
	// The AbilitySystemComponent may not have been valid when we first bound input... try again.
	if (!ProtelumAbilitySystem.IsValid())
	{
		RunAbilitySystemSetup();
	}

	if (ProtelumAbilitySystem.IsValid())
	{
		using namespace AbilityInputBindingComponent_Impl;

		const FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
		if (FoundBinding && ensure(FoundBinding->InputID != InvalidInputID))
		{
			ProtelumAbilitySystem->AbilityLocalInputPressed(FoundBinding->InputID);
		}
	}
}

void AProtelumPlayerController::OnAbilityInputReleased(TObjectPtr<UInputAction> InputAction)
{
	if (ProtelumAbilitySystem.IsValid())
	{
		using namespace AbilityInputBindingComponent_Impl;

		const FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
		if (FoundBinding && ensure(FoundBinding->InputID != InvalidInputID))
		{
			ProtelumAbilitySystem->AbilityLocalInputReleased(FoundBinding->InputID);
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AProtelumPlayerController::RequestMove(const FInputActionInstance& InputActionValue)
{
	const FVector2D MoveValue = InputActionValue.GetValue().Get<FVector2D>();
	if(IsValid(ProtelumCharacter))
	{
		ProtelumCharacter->OnMoveRequest(MoveValue);
	}
	
}
// ReSharper disable once CppMemberFunctionMayBeConst
void AProtelumPlayerController::RequestLook(const FInputActionInstance& InputActionValue)
{
	const FVector2D LookValue = InputActionValue.GetValue().Get<FVector2D>();
	if(IsValid(ProtelumCharacter))
	{
		ProtelumCharacter->OnLookRequest(LookValue);
	}
}
// ReSharper disable once CppMemberFunctionMayBeConst
void AProtelumPlayerController::RequestConfirmAbility(const FInputActionInstance& InputActionValue)
{
	if(IsValid(ProtelumCharacter))
	{
		ProtelumCharacter->OnConfirmRequest();
	}
	if(IsValid(InputSubsystem))
	{
		InputSubsystem->RemoveMappingContext(ConfirmCancelIMC.Get()); //Todo: if different GA is called CancelAbility
	}
}
// ReSharper disable once CppMemberFunctionMayBeConst
void AProtelumPlayerController::RequestCancelAbility(const FInputActionInstance& InputActionValue)
{
	if(IsValid(ProtelumCharacter))
	{
		ProtelumCharacter->OnCancelRequest();
	}
	if(IsValid(InputSubsystem))
	{
		InputSubsystem->RemoveMappingContext(ConfirmCancelIMC.Get()); //Todo: if different GA is called CancelAbility
	}
}

void AProtelumPlayerController::ResetBindings()
{
	for (auto& InputBinding : MappedAbilities)
	{
		if(IsValid(EnhancedInputComponent))
		{
			EnhancedInputComponent->RemoveBindingByHandle(InputBinding.Value.OnPressedHandle);
			EnhancedInputComponent->RemoveBindingByHandle(InputBinding.Value.OnReleasedHandle);
		}
		if(ProtelumAbilitySystem.IsValid())
		{
			const int32 ExpectedInputID = InputBinding.Value.InputID;

			for(const FGameplayAbilitySpecHandle AbilitySpecHandle : InputBinding.Value.BoundAbilitiesStack)
			{
				const TObjectPtr<FGameplayAbilitySpec> FoundAbility = ProtelumAbilitySystem->FindAbilitySpecFromHandle(AbilitySpecHandle);
				if(FoundAbility && FoundAbility->InputID == ExpectedInputID)
				{
					FoundAbility->InputID = AbilityInputBindingComponent_Impl::InvalidInputID;
				}
			}
		}
	}
	ProtelumAbilitySystem = nullptr;
}

void AProtelumPlayerController::RemoveEntry(TObjectPtr<UInputAction> InputAction)
{
	if(const TObjectPtr<FAbilityInputBinding> InputBindings = MappedAbilities.Find(InputAction))
	{
		if(IsValid(EnhancedInputComponent))
		{
			EnhancedInputComponent->RemoveBindingByHandle(InputBindings->OnPressedHandle);
			EnhancedInputComponent->RemoveBindingByHandle(InputBindings->OnReleasedHandle);
		}

		for(const FGameplayAbilitySpecHandle AbilitySpecHandle : InputBindings->BoundAbilitiesStack)
		{
			using namespace  AbilityInputBindingComponent_Impl;

			const TObjectPtr<FGameplayAbilitySpec> AbilitySpec = FindAbilitySpec(AbilitySpecHandle);
			if(AbilitySpec && AbilitySpec->InputID == InputBindings->InputID)
			{
				AbilitySpec->InputID = InvalidInputID;
			}
		}
		MappedAbilities.Remove(InputAction);
	}
}

TObjectPtr<FGameplayAbilitySpec> AProtelumPlayerController::FindAbilitySpec(
	FGameplayAbilitySpecHandle AbilitySpecHandle) const
{
	TObjectPtr<FGameplayAbilitySpec> FoundAbility = nullptr;
	if(ProtelumAbilitySystem.IsValid())
	{
		FoundAbility = ProtelumAbilitySystem->FindAbilitySpecFromHandle(AbilitySpecHandle);
	}
	return FoundAbility;
}