// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CharacterAttributeSet.generated.h"


// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:

protected:

	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);

	UFUNCTION()
	virtual void OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel);
	
	UFUNCTION()
	virtual void OnRep_XP(const FGameplayAttributeData& OldXP);
	
	UFUNCTION()
	virtual void OnRep_XPBounty(const FGameplayAttributeData& OldXPBounty);
	
	UFUNCTION()
	virtual void OnRep_Gold(const FGameplayAttributeData& OldGold);
	
	UFUNCTION()
	virtual void OnRep_GoldBounty(const FGameplayAttributeData& OldGoldBounty);

	//Adjusts an Attribute to fit the Percentage it had before the MaxAttribute change
	void AdjustAttributeForMaxChange(const FGameplayAttributeData & AffectedAttribute, const FGameplayAttributeData & MaxAttribute, float NewMaxValue, const FGameplayAttribute & AffectedAttributeProperty) const;
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Clamping and other changes that might be mandatory for the Attribute just before it gets applied ...
	//anything that recalculates the CurrentValue from all of the modifiers like
	//GameplayEffectExecutionCalculations and ModifierMagnitudeCalculations need to implement clamping again.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	//Base Value changes of Instant Gameplay Effects should be adjusted right here
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	//Used for example: Only strongest slow effect applies
	virtual void OnAttributeAggregatorCreated(const FGameplayAttribute& Attribute, FAggregator* NewAggregator) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Level", ReplicatedUsing = OnRep_CharacterLevel)
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, CharacterLevel)

	UPROPERTY(BlueprintReadOnly, Category = "Level", ReplicatedUsing = OnRep_XP)
	FGameplayAttributeData XP;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, XP)

	UPROPERTY(BlueprintReadOnly, Category = "Level", ReplicatedUsing = OnRep_XPBounty)
	FGameplayAttributeData XPBounty;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, XPBounty)

	UPROPERTY(BlueprintReadOnly, Category = "Money", ReplicatedUsing = OnRep_Gold)
	FGameplayAttributeData Gold;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Gold)

	UPROPERTY(BlueprintReadOnly, Category = "Money", ReplicatedUsing = OnRep_GoldBounty)
	FGameplayAttributeData GoldBounty;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, GoldBounty)

private:
};
