// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtelumBaseClasses/ProtelumPlayerState.h"
#include "AbilitySystem/ProtelumAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/AttackerAttributeSet.h"
#include "AbilitySystem/AttributeSets/CharacterAttributeSet.h"
#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"
#include "AbilitySystem/AttributeSets/ShamanWeaponAttributeSet.h"
#include "ProtelumBaseClasses/ProtelumCharacter.h"
#include "Subsystems/UISubsystem.h"

AProtelumPlayerState::AProtelumPlayerState()
{
	ProtelumAbilitySystem = CreateDefaultSubobject<UProtelumAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	if(IsValid(ProtelumAbilitySystem))
	{
		ProtelumAbilitySystem->SetIsReplicated(true);
		ProtelumAbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	}
	CharacterAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
	HealthAttributeSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"));
	ShamanWeaponAttributeSet = CreateDefaultSubobject<UShamanWeaponAttributeSet>(TEXT("ShamanWeaponAttributeSet"));
	AttackerAttributeSet = CreateDefaultSubobject<UAttackerAttributeSet>(TEXT("AttackerAttributeSet"));
	
	NetUpdateFrequency = 100.0f;
	//Activate Adaptive Network Update Frequency
	//Todo: Initialize a Dead Tag and maybe a Down Tag
	MaxHealAmmo = FGameplayTag::RequestGameplayTag("Data.Shaman.Ammo.MaxHeal");
	MaxDamageAmmo = FGameplayTag::RequestGameplayTag("Data.Shaman.Ammo.MaxDamage");
}

// void AProtelumPlayerState::ClientInitialize(AController* C)
// {
// 	Super::ClientInitialize(C);
// 	if(C != nullptr && C->IsLocalPlayerController())
// 	{
// 		const TWeakObjectPtr<UGameInstance> GameInstance = GetGameInstance();
// 		if(GameInstance.IsValid())
// 		{
// 			const TWeakObjectPtr<UUISubsystem> UISubsystem = GameInstance->GetSubsystem<UUISubsystem>();
// 			if(UISubsystem.IsValid())
// 			{
// 				UISubsystem->LoadInterfaceBaseWidget(EBaseWidgetType::None);
// 			}
// 		}
// 	}
// }

void AProtelumPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(ProtelumAbilitySystem))
	{
		//Begin Binding Delegates On  CharacterAttributeValueChanged
		MoveSpeedChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(CharacterAttributeSet->GetMoveSpeedAttribute()).AddUObject(this, &AProtelumPlayerState::MoveSpeedChanged);
		CharacterLevelChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(CharacterAttributeSet->GetCharacterLevelAttribute()).AddUObject(this, &AProtelumPlayerState::CharacterLevelChanged);
		XPChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(CharacterAttributeSet->GetXPAttribute()).AddUObject(this, &AProtelumPlayerState::XPChanged);
		XPBountyChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(CharacterAttributeSet->GetXPBountyAttribute()).AddUObject(this, &AProtelumPlayerState::XPBountyChanged);
		GoldChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(CharacterAttributeSet->GetGoldAttribute()).AddUObject(this, &AProtelumPlayerState::GoldChanged);
		GoldBountyChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(CharacterAttributeSet->GetGoldBountyAttribute()).AddUObject(this, &AProtelumPlayerState::GoldBountyChanged);
		//End Binding Delegates On CharacterAttributeValueChanged

		//Begin Binding Delegates On HealthAttributeValueChanged
		HealthChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).AddUObject(this, &AProtelumPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &AProtelumPlayerState::MaxHealthChanged);
		HealthRegenerationRateChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthRegenerationRateAttribute()).AddUObject(this, &AProtelumPlayerState::HealthRegenerationRateChanged);
		ShieldChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetShieldAttribute()).AddUObject(this, &AProtelumPlayerState::ShieldChanged);
		DamageResistanceDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetDamageResistanceAttribute()).AddUObject(this, &AProtelumPlayerState::DamageResistanceChanged);
		CriticalHitResistanceChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetCriticalHitResistanceAttribute()).AddUObject(this, &AProtelumPlayerState::CriticalHitResistanceChanged);
		ShieldRegenerationRateChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetShieldRegenerationRateAttribute()).AddUObject(this, &AProtelumPlayerState::ShieldRegenerationRateChanged);
		DamageVulnerabilityChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetDamageVulnerabilityAttribute()).AddUObject(this, &AProtelumPlayerState::DamageVulnerabilityChanged);
		//End Binding Delegates On HealthAttributeValueChanged

		//Begin Binding Delegates On AttackerAttributeValueChanged
		CriticalHitChanceChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(AttackerAttributeSet->GetCriticalHitChanceAttribute()).AddUObject(this, &AProtelumPlayerState::CriticalHitChanceChanged);
		ShieldPenetrationChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(AttackerAttributeSet->GetShieldPenetrationAttribute()).AddUObject(this, &AProtelumPlayerState::ShieldPenetrationChanged);
		ResistancePenetrationChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(AttackerAttributeSet->GetResistancePenetrationAttribute()).AddUObject(this, &AProtelumPlayerState::ResistancePenetrationChanged);
		//End Binding Delegates On AttackerAttributeValueChanged
		
		//Begin Binding Delegates On ShamanWeaponAttributeValueChanged
        DamageProjectileMaxAmmoChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(ShamanWeaponAttributeSet->GetDamageProjectileMaxAmmoAttribute()).AddUObject(this, &AProtelumPlayerState::DamageProjectileMaxAmmoChanged);
        DamageProjectileAmmoChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(ShamanWeaponAttributeSet->GetDamageProjectileAmmoAttribute()).AddUObject(this, &AProtelumPlayerState::DamageProjectileAmmoChanged);
        DamageProjectileRegenerationChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(ShamanWeaponAttributeSet->GetDamageProjectileRegenerationAttribute()).AddUObject(this, &AProtelumPlayerState::DamageProjectileRegenerationChanged);
        ProjectileDamageChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(ShamanWeaponAttributeSet->GetProjectileDamageAttribute()).AddUObject(this, &AProtelumPlayerState::ProjectileDamageChanged);
        HealProjectileMaxAmmoChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(ShamanWeaponAttributeSet->GetHealProjectileMaxAmmoAttribute()).AddUObject(this, &AProtelumPlayerState::HealProjectileMaxAmmoChanged);
        HealProjectileAmmoChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(ShamanWeaponAttributeSet->GetHealProjectileAmmoAttribute()).AddUObject(this, &AProtelumPlayerState::HealProjectileAmmoChanged);
        HealProjectileRegenerationChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(ShamanWeaponAttributeSet->GetHealProjectileRegenerationAttribute()).AddUObject(this, &AProtelumPlayerState::HealProjectileRegenerationChanged);
        ProjectileHealChangedDelegateHandle = ProtelumAbilitySystem->GetGameplayAttributeValueChangeDelegate(ShamanWeaponAttributeSet->GetProjectileHealAttribute()).AddUObject(this, &AProtelumPlayerState::ProjectileHealChanged);
		//End Binding Delegates On ShamanWeaponAttributeValueChanged
	}
}

void AProtelumPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	
}

void AProtelumPlayerState::HealthRegenerationRateChanged(const FOnAttributeChangeData& Data)
{
	
}

void AProtelumPlayerState::ShieldChanged(const FOnAttributeChangeData& Data)
{
	// UE_LOG(LogTemp, Warning , TEXT("Shield %f"), Data.NewValue);
}

void AProtelumPlayerState::DamageResistanceChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::CriticalHitResistanceChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::ShieldRegenerationRateChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::DamageVulnerabilityChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::CriticalHitChanceChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::ShieldPenetrationChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::ResistancePenetrationChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::MoveSpeedChanged(const FOnAttributeChangeData& Data)
{
	const TWeakObjectPtr<AProtelumCharacter> ProtelumCharacter = Cast<AProtelumCharacter>(GetPawn());
	//Todo: Init somewhere else
	if(ProtelumCharacter.IsValid())
	{
		ProtelumCharacter->OnRequestSetMoveSpeed(Data.NewValue);
	}
}

void AProtelumPlayerState::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::XPChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::XPBountyChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::GoldChanged(const FOnAttributeChangeData& Data)
{
	
}

void AProtelumPlayerState::GoldBountyChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::DamageProjectileMaxAmmoChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::DamageProjectileAmmoChanged(const FOnAttributeChangeData& Data)
{
	// UE_LOG(LogTemp, Warning , TEXT("%s %s() Damage Ammo %f"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__), Data.NewValue);
}

void AProtelumPlayerState::DamageProjectileRegenerationChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::ProjectileDamageChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::HealProjectileMaxAmmoChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::HealProjectileAmmoChanged(const FOnAttributeChangeData& Data)
{
	// UE_LOG(LogTemp, Warning , TEXT("%s %s() Heal Ammo %f"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__), Data.NewValue);
}

void AProtelumPlayerState::HealProjectileRegenerationChanged(const FOnAttributeChangeData& Data)
{
}

void AProtelumPlayerState::ProjectileHealChanged(const FOnAttributeChangeData& Data)
{
}

float AProtelumPlayerState::GetMoveSpeed() const
{
	if(IsValid(CharacterAttributeSet))
	{
		return CharacterAttributeSet->GetMoveSpeed();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetCharacterLevel() const
{
	if(IsValid(CharacterAttributeSet))
	{
		return CharacterAttributeSet->GetCharacterLevel();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetXP() const
{
	if(IsValid(CharacterAttributeSet))
	{
		return CharacterAttributeSet->GetXP();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetXPBounty() const
{
	if(IsValid(CharacterAttributeSet))
	{
		return CharacterAttributeSet->GetXPBounty();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetGold() const
{
	if(IsValid(CharacterAttributeSet))
	{
		return CharacterAttributeSet->GetGold();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetGoldBounty() const
{
	if(IsValid(CharacterAttributeSet))
	{
		return CharacterAttributeSet->GetGoldBounty();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetHealth() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetHealth();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetMaxHealth() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetHealthRegenerationRate() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetHealthRegenerationRate();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetShield() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetShield();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetDamageResistance() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetDamageResistance();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetShieldRegenerationRate() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetShieldRegenerationRate();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetCriticalHitResistance() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetCriticalHitResistance();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetDamageVulnerability() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetDamageVulnerability();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetDamageProjectileMaxAmmo() const
{
	if(IsValid(ShamanWeaponAttributeSet))
	{
		return ShamanWeaponAttributeSet->GetDamageProjectileMaxAmmo();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetDamageProjectileAmmo() const
{
	if(IsValid(ShamanWeaponAttributeSet))
	{
		return ShamanWeaponAttributeSet->GetDamageProjectileAmmo();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetDamageProjectileRegeneration() const
{
	if(IsValid(ShamanWeaponAttributeSet))
	{
		return ShamanWeaponAttributeSet->GetDamageProjectileRegeneration();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetProjectileDamage() const
{
	if(IsValid(ShamanWeaponAttributeSet))
	{
		return ShamanWeaponAttributeSet->GetProjectileDamage();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetHealProjectileMaxAmmo() const
{
	if(IsValid(ShamanWeaponAttributeSet))
	{
		return ShamanWeaponAttributeSet->GetHealProjectileMaxAmmo();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetHealProjectileAmmo() const
{
	if(IsValid(ShamanWeaponAttributeSet))
	{
		return ShamanWeaponAttributeSet->GetHealProjectileAmmo();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetHealProjectileRegeneration() const
{
	if(IsValid(ShamanWeaponAttributeSet))
	{
		return ShamanWeaponAttributeSet->GetDamageProjectileRegeneration();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetProjectileHeal() const
{
	if(IsValid(ShamanWeaponAttributeSet))
	{
		return ShamanWeaponAttributeSet->GetProjectileHeal();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetCriticalHitChance() const
{
	if(IsValid(AttackerAttributeSet))
	{
		return AttackerAttributeSet->GetCriticalHitChance();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetShieldPenetration() const
{
	if(IsValid(AttackerAttributeSet))
	{
		return AttackerAttributeSet->GetShieldPenetration();
	}
	return 0.0f;
}

float AProtelumPlayerState::GetResistancePenetration() const
{
	if(IsValid(AttackerAttributeSet))
	{
		return AttackerAttributeSet->GetResistancePenetration();
	}
	return 0.0f;
}

UAbilitySystemComponent* AProtelumPlayerState::GetAbilitySystemComponent() const
{
	return ProtelumAbilitySystem;
}

TWeakObjectPtr<UCharacterAttributeSet> AProtelumPlayerState::GetCharacterAttributeSet() const
{
	return CharacterAttributeSet;
}

TWeakObjectPtr<UHealthAttributeSet> AProtelumPlayerState::GetHealthAttributeSet() const
{
	return HealthAttributeSet;
}

TWeakObjectPtr<UAttackerAttributeSet> AProtelumPlayerState::GetAttackerAttributeSet() const
{
	return AttackerAttributeSet;
}

TWeakObjectPtr<UShamanWeaponAttributeSet> AProtelumPlayerState::GetShamanWeaponAttributeSet() const
{
	return ShamanWeaponAttributeSet;
}
