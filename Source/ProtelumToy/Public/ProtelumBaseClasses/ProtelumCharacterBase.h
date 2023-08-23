// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Interface/AbilityBindingInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/DefaultDataAssets/AbilityBindingDataAsset.h"
#include "AbilitySystem/DefaultDataAssets/AbilityLibraryDataAsset.h"
#include "ProtelumCharacterBase.generated.h"

class UAttackerAttributeSet;
class UHealthAttributeSet;
class UShamanWeaponAttributeSet;
class UStaticActorAttributeSet;
class AProtelumPlayerController;
class UProtelumGameplayAbility;
class UCharacterAttributeSet;
class UProtelumAbilitySystemComponent;
class UAbilityLibraryDataAsset;
class UAbilityBindingDataAsset;
class UGameplayEffect;
UCLASS()
class PROTELUMTOY_API AProtelumCharacterBase : public ACharacter, public IAbilitySystemInterface, public IAbilityBindingInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this character's properties
	AProtelumCharacterBase();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//The ASC gets initialized here
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;
	
	//Begin AbilityBindingInterface
	virtual void BindAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void UnbindAbility(FGameplayAbilitySpec& AbilitySpec) override;
	//End AbilityBindingInterface

	
	//Begin AbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//End AbilitySystemInterface

	//Begin InputCallback
	void OnMoveRequest(const FVector2D MoveValue);
	void OnLookRequest(const FVector2D LookValue);
	void OnConfirmRequest() const;
	void OnCancelRequest() const;
	//End InputCallback

	//Begin AttributeChangedCallbacks
	void OnRequestSetMoveSpeed(float NewMoveSpeed) const;
	//End AttributeChangedCallback
	
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

	bool IsAlive() const;

	//Begin Global Setter and Getter To Modify Bound Abilities
	void SetAbilitySlotToModify(FGameplayTag AbilitySlotToModify);
	FGameplayTag GetAbilitySlotToModify();
	void SetAbilityToReplace(FGameplayTag AbilityToReplace);
	FGameplayTag GetAbilityToReplace();
	//End Global Setter and Getter To Modify Bound Abilities

	//Is only here Because the DebugCommand needs it right now
	TSubclassOf<UProtelumGameplayAbility> FindOwnableAbilityByTag(const FGameplayTag GameplayTag) const;

	FGameplayAbilitySpec* FindAbilitySpecBySlotTag(const FGameplayTag SlotTag);

	FGameplayAbilitySpec* FindAbilitySpecByHandle(const FGameplayAbilitySpecHandle SpecHandle) const;
	
	void IncreaseAbilityLevelBySlot(const FGameplayTag& SlotToLevel);
	void DecreaseAbilityLevelBySlot(const FGameplayTag& SlotToLevel);
protected:
	TWeakObjectPtr<AProtelumPlayerController> ProtelumPlayerController = {nullptr};
	TWeakObjectPtr<UProtelumAbilitySystemComponent> ProtelumAbilitySystem = {nullptr};
private:
	
	//With this Effect we Initialize all the Attributes we need with a GameplayEffect
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> DefaultAttributeInitializeEffects = {nullptr};

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects = {nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UProtelumGameplayAbility>> StartupBaseAttackAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UProtelumGameplayAbility>> AlwaysActiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UProtelumGameplayAbility>> AbilitiesWithoutInputBindingToLoadOnStartup;
	
	//This Asset Structures the AbilityBinding in BeginPlay on Game first start
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TObjectPtr<UAbilityBindingDataAsset> AbilityBindingAsset = {nullptr};
	//This Asset Structures the Ownable Abilities which will not change during Game
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TObjectPtr<UAbilityLibraryDataAsset> OwnableAbilityAsset = {nullptr};

	UPROPERTY()//Only on Server Cheap way to Store the OwnableAbilities
	FGameplayTagContainer OwnableAbilityClassTags;// Maybe FOwnedAbilities, if we ever want to change it

	
	//This is the Runtime BoundAbilityArray, Before ASC->GiveAbility we want to change the BoundAbilitiesArray
	UPROPERTY(Replicated)
	TArray<FGameplayAbilityInfo> BoundAbilities;

	//This Ability will be called on Empty AbilitySlots to Prevent Null and to inform Player, and to know that we can fill it
	UPROPERTY(Replicated)
	TSubclassOf<UProtelumGameplayAbility> EmptyGameplayAbilityClass = {nullptr};
	
	//This will be filled with life when saving a game and reloading it (Does nothing right now
	TObjectPtr<UAbilityBindingDataAsset> SavedAbilities = {nullptr}; //Container saves and loads changed abilities that were Bound;

	
	TWeakObjectPtr<UCharacterAttributeSet> CharacterAttributeSet = {nullptr};
	TWeakObjectPtr<UHealthAttributeSet> HealthAttributeSet = {nullptr};
	TWeakObjectPtr<UShamanWeaponAttributeSet> ShamanWeaponAttributeSet = {nullptr}; //Todo maybe solve this with Inheritance
	TWeakObjectPtr<UAttackerAttributeSet> AttackerAttributeSet = {nullptr};


	//Begin Ability Binding Functions and Variables
	
	//GlobalVariable to Swap a TargetedAbilitySlots Ability
	UPROPERTY()
	FGameplayTag TargetedAbilitySlot = FGameplayTag::EmptyTag;
	//GlobalVariable to Replace an SpecificAbility
	UPROPERTY()
	FGameplayTag TargetedAbility = FGameplayTag::EmptyTag;
	bool bReplaceSpecificAbility = false;

	
	bool IsGivenAbilityOwnable(const FGameplayAbilitySpec& AbilitySpec) const;
	void ModifyBoundAbilities(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag FindAbilitySlotByDefaultObject(TSubclassOf<UProtelumGameplayAbility> AbilityToFind);
	// TSubclassOf<UGameplayAbility> FindOwnableAbilityByTag(const FGameplayTag GameplayTag) const;
	FGameplayTag IsAbilityAlreadyBound(const FGameplayAbilitySpec& AbilitySpec, bool& bFoundBoundAbility);
	//This should only be called when it is safe to do so There is a slot with this
	//GameplayTag and there is any Ability bound
	FGameplayAbilitySpecHandle ReplaceBoundAbilityBySlot(const FGameplayTag SlotToFill,
	                                                     const FGameplayAbilitySpec& AbilitySpec);
	void BindAbilityToInput(const FGameplayAbilitySpec& AbilitySpecToBind);
	//End Ability Binding Functions and Variables
	
	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeDefaultAttributes();

	virtual void ApplyStartupEffects();

	//(Re)Loading the BoundAbilityArray
	void LoadBoundAbilities();
	bool bBoundAbilitiesLoaded = false;

	//(Re)Loading the OwnableAbility GameplayTagContainer
	void LoadOwnableAbilities();
	bool bOwnableAbilitiesLoaded = false;
	//Adding the CharacterStartAbilities
	void AddCharacterStartAbilities();
	bool bCharacterStartAbilitiesGiven = false;

	void AddBaseAttackAbilities();
	void AddAlwaysActiveAbilities();
	void AddAbilitiesWithoutInputBindingToLoadOnStartup();
};
