// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameplayAbilitySpec.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProtelumPlayerController.generated.h"


class UProtelumAbilitySystemComponent;
class AProtelumCharacter;
class UEnhancedInputLocalPlayerSubsystem;

USTRUCT()
struct FAbilityInputBinding
{
	GENERATED_BODY()
	int32 InputID = 0;
	uint32 OnPressedHandle = 0;
	uint32 OnReleasedHandle = 0;
	TArray<FGameplayAbilitySpecHandle> BoundAbilitiesStack;
};

/**
 * 
 */


UCLASS()
class PROTELUMTOY_API AProtelumPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AProtelumPlayerController();
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void OnRep_Pawn() override;
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	// bool IsConfirmCancel(const TObjectPtr<UInputAction>& Object) const;
	void SetInputBinding(TObjectPtr<UInputAction> InputAction, FGameplayAbilitySpecHandle AbilitySpecHandle);
	void ClearInputBinding(FGameplayAbilitySpecHandle AbilitySpecHandle);
	void ClearAbilityBindings(TObjectPtr<UInputAction> InputAction);
	
	void OnWaitConfirmInputBegin() const;
	void OnWaitConfirmInputEnd() const;
private:

	bool bIMCConfirmCancelBound = false;
	
	TObjectPtr<AProtelumCharacter> ProtelumCharacter = {nullptr};

	UPROPERTY(Transient)
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem = {nullptr};

	UPROPERTY(Transient)
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = {nullptr};

	UPROPERTY(Transient)
	TWeakObjectPtr<UProtelumAbilitySystemComponent> ProtelumAbilitySystem = {nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IAMove = {nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IALook = {nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IAConfirm = {nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IACancel = {nullptr};
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> DefaultIMC;
	
	//This will be added OnWaitingForConfirmInputBegin();
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> ConfirmCancelIMC;

	UPROPERTY(Transient)
	TMap<TObjectPtr<UInputAction>, FAbilityInputBinding> MappedAbilities;
	
	void RunAbilitySystemSetup();
	
	void TryBindAbilityInput(TObjectPtr<UInputAction> InputAction, FAbilityInputBinding& AbilityInputBinding);

	void OnAbilityInputPressed(TObjectPtr<UInputAction> InputAction);
	void OnAbilityInputReleased(TObjectPtr<UInputAction> InputAction);
	
	void RequestMove(const FInputActionInstance& InputActionValue);
	void RequestLook(const FInputActionInstance& InputActionValue);
	void RequestConfirmAbility(const FInputActionInstance& InputActionValue);
	void RequestCancelAbility(const FInputActionInstance& InputActionValue);

	void ResetBindings();
	void RemoveEntry(TObjectPtr<UInputAction> InputAction);
	
	TObjectPtr<FGameplayAbilitySpec> FindAbilitySpec(FGameplayAbilitySpecHandle AbilitySpecHandle) const;

	
	
};



