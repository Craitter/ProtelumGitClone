// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "ProtelumPlayerState.generated.h"

class UAttackerAttributeSet;
class UHealthAttributeSet;
class UShamanWeaponAttributeSet;
class AProtelumCharacter;
class UProtelumAbilitySystemComponent;
class UCharacterAttributeSet;
struct FOnAttributeChangeData;
/**
 * 
 */
UCLASS()
class PROTELUMTOY_API AProtelumPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	AProtelumPlayerState();
protected:
	

	virtual void BeginPlay() override;
	//Begin CHarcter AttributeChangedDelegate
	FDelegateHandle MoveSpeedChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;
	FDelegateHandle XPChangedDelegateHandle;
	FDelegateHandle XPBountyChangedDelegateHandle;
	FDelegateHandle GoldChangedDelegateHandle;
	FDelegateHandle GoldBountyChangedDelegateHandle;
	//End Character AttributeChangedDelegate
	
	//Begin Character AttributeChangedCallbacks
	virtual void MoveSpeedChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);
	virtual void XPChanged(const FOnAttributeChangeData& Data);
	virtual void XPBountyChanged(const FOnAttributeChangeData& Data);
	virtual void GoldChanged(const FOnAttributeChangeData& Data);
	virtual void GoldBountyChanged(const FOnAttributeChangeData& Data);
	//End Character AttributeChangedCallbacks

	//Begin Health AttributeChangedDelegate
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle HealthRegenerationRateChangedDelegateHandle;
	FDelegateHandle ShieldChangedDelegateHandle;
	FDelegateHandle DamageResistanceDelegateHandle;
	FDelegateHandle CriticalHitResistanceChangedDelegateHandle;
	FDelegateHandle ShieldRegenerationRateChangedDelegateHandle;
	FDelegateHandle DamageVulnerabilityChangedDelegateHandle;
	//End Health AttributeChangedDelegate
	
	//Begin Health AttributeChangedCallbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void HealthRegenerationRateChanged(const FOnAttributeChangeData& Data);
	virtual void ShieldChanged(const FOnAttributeChangeData& Data);
	virtual void DamageResistanceChanged(const FOnAttributeChangeData& Data);
	virtual void CriticalHitResistanceChanged(const FOnAttributeChangeData& Data);
	virtual void ShieldRegenerationRateChanged(const FOnAttributeChangeData& Data);
	virtual void DamageVulnerabilityChanged(const FOnAttributeChangeData& Data);
	//End Health AttributeChangedCallbacks

	//Begin Attacker AttributeChangedDelegate
	FDelegateHandle CriticalHitChanceChangedDelegateHandle;
	FDelegateHandle ShieldPenetrationChangedDelegateHandle;
	FDelegateHandle ResistancePenetrationChangedDelegateHandle;
	//End Attacker AttributeChangedDelegate
	
	//Begin Attacker AttributeChangedCallbacks
	virtual void CriticalHitChanceChanged(const FOnAttributeChangeData& Data);
	virtual void ShieldPenetrationChanged(const FOnAttributeChangeData& Data);
	virtual void ResistancePenetrationChanged(const FOnAttributeChangeData& Data);
	//End Attacker AttributeChangedCallbacks
		
	//Begin ShamanWeapon AttributeChangedDelegate
	FDelegateHandle DamageProjectileMaxAmmoChangedDelegateHandle;
	FDelegateHandle DamageProjectileAmmoChangedDelegateHandle;
	FDelegateHandle DamageProjectileRegenerationChangedDelegateHandle;
	FDelegateHandle ProjectileDamageChangedDelegateHandle;
	FDelegateHandle HealProjectileMaxAmmoChangedDelegateHandle;
	FDelegateHandle HealProjectileAmmoChangedDelegateHandle;
	FDelegateHandle HealProjectileRegenerationChangedDelegateHandle;
	FDelegateHandle ProjectileHealChangedDelegateHandle;
	//End ShamanWeapon AttributeChangedDelegate
	
	//Begin ShamanWeapon AttributeChangedCallbacks
	virtual void DamageProjectileMaxAmmoChanged(const FOnAttributeChangeData& Data);
	virtual void DamageProjectileAmmoChanged(const FOnAttributeChangeData& Data);
	virtual void DamageProjectileRegenerationChanged(const FOnAttributeChangeData& Data);
	virtual void ProjectileDamageChanged(const FOnAttributeChangeData& Data);
	virtual void HealProjectileMaxAmmoChanged(const FOnAttributeChangeData& Data);
	virtual void HealProjectileAmmoChanged(const FOnAttributeChangeData& Data);
	virtual void HealProjectileRegenerationChanged(const FOnAttributeChangeData& Data);
	virtual void ProjectileHealChanged(const FOnAttributeChangeData& Data);
	//End ShamanWeapon AttributeChangedCallbacks

	FGameplayTag MaxHealAmmo;
	FGameplayTag MaxDamageAmmo;
public:
	//Begin Character Attribute Getter
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Character")
	float GetMoveSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Character")
	float GetCharacterLevel() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Character")
	float GetXP() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Character")
	float GetXPBounty() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Character")
	float GetGold() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Character")
	float GetGoldBounty() const;
	//End Character Attribute Getter

	//Begin Health Attribute Getter
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Health")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Health")
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Health")
	float GetHealthRegenerationRate() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Health")
	float GetShield() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Health")
	float GetDamageResistance() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Health")
	float GetShieldRegenerationRate() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Health")
	float GetCriticalHitResistance() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Health")
	float GetDamageVulnerability() const;
	//End Health Attribute Getter
	
	//Begin Shaman Attribute Getter
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|ShamanWeapon")
	float GetDamageProjectileMaxAmmo() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|ShamanWeapon")
	float GetDamageProjectileAmmo() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|ShamanWeapon")
	float GetDamageProjectileRegeneration() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|ShamanWeapon")
	float GetProjectileDamage() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|ShamanWeapon")
	float GetHealProjectileMaxAmmo() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|ShamanWeapon")
	float GetHealProjectileAmmo() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|ShamanWeapon")
	float GetHealProjectileRegeneration() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|ShamanWeapon")
	float GetProjectileHeal() const;
	//End Shaman Attribute Getter

	//Begin Attacker Attribute Getter
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Attacker")
	float GetCriticalHitChance() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Attacker")
	float GetShieldPenetration() const;
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Attacker")
	float GetResistancePenetration() const;
	//End Attacker Attribute Getter
	
	//Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//End IAbilitySystemInterface
	
	TWeakObjectPtr<UCharacterAttributeSet> GetCharacterAttributeSet() const;

	TWeakObjectPtr<UHealthAttributeSet> GetHealthAttributeSet() const;

	TWeakObjectPtr<UAttackerAttributeSet> GetAttackerAttributeSet() const;

	TWeakObjectPtr<UShamanWeaponAttributeSet> GetShamanWeaponAttributeSet() const;
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCharacterAttributeSet> CharacterAttributeSet = {nullptr};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthAttributeSet> HealthAttributeSet = {nullptr};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShamanWeaponAttributeSet> ShamanWeaponAttributeSet = {nullptr};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttackerAttributeSet> AttackerAttributeSet = {nullptr};
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProtelumAbilitySystemComponent> ProtelumAbilitySystem = {nullptr};
};
