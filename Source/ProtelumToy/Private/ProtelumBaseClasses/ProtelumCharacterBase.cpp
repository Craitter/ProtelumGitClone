// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtelumBaseClasses/ProtelumCharacterBase.h"

#include "AbilitySystem/ProtelumAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/AttackerAttributeSet.h"
#include "AbilitySystem/AttributeSets/CharacterAttributeSet.h"
#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"
#include "AbilitySystem/AttributeSets/ShamanWeaponAttributeSet.h"
#include "AbilitySystem/GameplayAbility/ProtelumGameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Net/UnrealNetwork.h"
#include "ProtelumBaseClasses/ProtelumPlayerController.h"
#include "ProtelumBaseClasses/ProtelumPlayerState.h"


static FAutoConsoleCommand CVarChangeAbility(TEXT("AbilitySystem.GiveAbilityToCharacter"), TEXT("None\n"),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
	{
		FString ServerCommand = TEXT("AbilitySystem.GiveAbilityToCharacter");
		//New Ability
		if(!Args.IsValidIndex(0))
		{
			UE_LOG(LogTemp, Warning , TEXT("%s() You need to define a NewAbility"), *FString(__FUNCTION__));
			return;
		}
		FString NewAbilityString = TEXT("AbilityClasses.");
		NewAbilityString += Args[0];

		//SlotToFill
		if(!Args.IsValidIndex(1))
		{
			UE_LOG(LogTemp, Warning , TEXT(" %s() You need to define a Slot To Add the Ability to, if you dont want to set a slot, Type in \"0\""), *FString(__FUNCTION__));
			return;
		}
		bool bHasAbilitySlot = true;
		if(UKismetStringLibrary::Conv_StringToInt(Args[1]) == 0)
		{
			bHasAbilitySlot = false;
		}
		FString SlotToFillString = TEXT("AbilitySlot.");
		SlotToFillString += Args[1];
		
		//OldAbility
		if(!Args.IsValidIndex(2))
		{
			UE_LOG(LogTemp, Warning , TEXT(" %s() You Need to define the Ability you want to Remove for that, if you dont want to Remove a specific Ability, Type in \"0\""), *FString(__FUNCTION__));
			return;
		}
		bool bRemoveOldAbility = true;
		if(UKismetStringLibrary::Conv_StringToInt(Args[2]) == 0 || bHasAbilitySlot)
		{
			bRemoveOldAbility = false;
		}
		FString OldAbilityString = TEXT("AbilityClasses.");
		OldAbilityString += Args[2];
		
		//PlayerNumber
		
		if(!Args.IsValidIndex(3))
		{
			UE_LOG(LogTemp, Warning , TEXT(" %s() You Need to define a Player to apply this to DefaultPlayer is \"0\""), *FString(__FUNCTION__));
			return;
		}
		int32 PlayerIndex = 0;
		

		check(GEngine)
		
		// GEngine->GetDebugLocalPlayer();

		const FWorldContext* Context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		// check(Context)
		const UWorld* World = Context->World();
		if(World == nullptr) return;
		APlayerController* PController = UGameplayStatics::GetPlayerController(World, PlayerIndex);
		if(PController == nullptr) return;
		if(PController->GetLocalRole() == ROLE_Authority)
		{
			PlayerIndex = UKismetStringLibrary::Conv_StringToInt(Args[3]);
			PController = UGameplayStatics::GetPlayerController(World, PlayerIndex);
			if(PController == nullptr) return;
		}
		AProtelumCharacterBase* GasCharacter = Cast<AProtelumCharacterBase>(PController->GetPawn());
		if(GasCharacter == nullptr) return;
		if(GasCharacter->GetLocalRole() != ROLE_Authority)
		{
			for(auto Argument : Args)
			{
				ServerCommand += " " + Argument;
			}
			UE_LOG(LogTemp, Warning , TEXT("%s %s() "), *ServerCommand, *FString(__FUNCTION__));
			if(PController == nullptr)
			{
				UE_LOG(LogTemp, Warning , TEXT("%s() PC is null"), *FString(__FUNCTION__));
				return;
			}
			PController->ServerExec(ServerCommand);
			return;
		}
		
		const FGameplayTag NewAbility = FGameplayTag::RequestGameplayTag(FName(*NewAbilityString), true);

		if(bHasAbilitySlot)
		{
			const FGameplayTag SlotToFill = FGameplayTag::RequestGameplayTag(FName(*SlotToFillString), false);
			GasCharacter->SetAbilitySlotToModify(SlotToFill);
		}

		if(bRemoveOldAbility)
		{
			const FGameplayTag OldAbility = FGameplayTag::RequestGameplayTag(FName(*OldAbilityString), false);
			GasCharacter->SetAbilityToReplace(OldAbility);
		}
		UAbilitySystemComponent* AbilitySystemComponent = GasCharacter->GetAbilitySystemComponent();
		if(AbilitySystemComponent == nullptr) return;
		FGameplayAbilitySpec TempSpec = FGameplayAbilitySpec(GasCharacter->FindOwnableAbilityByTag(NewAbility), 1.0f, INDEX_NONE);
		AbilitySystemComponent->GiveAbility(TempSpec);
		//Todo: Rework this command cause tags changed and level is not handled
	}),
	ECVF_Default);

// Sets default values
AProtelumCharacterBase::AProtelumCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

void AProtelumCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProtelumCharacterBase, BoundAbilities);

	DOREPLIFETIME(AProtelumCharacterBase, EmptyGameplayAbilityClass);
}

void AProtelumCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ProtelumPlayerController = Cast<AProtelumPlayerController>(NewController);
	const TObjectPtr<AProtelumPlayerState> PS = GetPlayerState<AProtelumPlayerState>();
	if(IsValid(PS))
	{
		ProtelumAbilitySystem = Cast<UProtelumAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		if(ProtelumAbilitySystem.IsValid())
		{
			ProtelumAbilitySystem->InitAbilityActorInfo(PS, this); 			
 			
			CharacterAttributeSet = PS->GetCharacterAttributeSet();
			ShamanWeaponAttributeSet = PS->GetShamanWeaponAttributeSet();
			HealthAttributeSet = PS->GetHealthAttributeSet();
			AttackerAttributeSet = PS->GetAttackerAttributeSet();
		}
	}
	if(IsValid(NewController))
	{
		if(NewController->IsPlayerController() && ProtelumAbilitySystem.IsValid())
		{
			ProtelumAbilitySystem->SetIsPlayerControlled();
		}
	}
	InitializeDefaultAttributes();
	ApplyStartupEffects();
	LoadBoundAbilities();
	LoadOwnableAbilities();
	AddCharacterStartAbilities();
}

void AProtelumCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	const TObjectPtr<AProtelumPlayerState> PS = GetPlayerState<AProtelumPlayerState>();
	if(IsValid(PS))
	{
		ProtelumAbilitySystem = Cast<UProtelumAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		if(ProtelumAbilitySystem.IsValid())
		{
			ProtelumAbilitySystem->InitAbilityActorInfo(PS, this); 			
 			
			CharacterAttributeSet = PS->GetCharacterAttributeSet();
			ShamanWeaponAttributeSet = PS->GetShamanWeaponAttributeSet();
			HealthAttributeSet = PS->GetHealthAttributeSet();
			AttackerAttributeSet = PS->GetAttackerAttributeSet();
		} 		
		InitializeDefaultAttributes();
		ApplyStartupEffects();
	}
}

void AProtelumCharacterBase::OnRep_Controller()
{
	Super::OnRep_Controller();
	ProtelumPlayerController = Cast<AProtelumPlayerController>(GetController());
}

void AProtelumCharacterBase::BindAbility(FGameplayAbilitySpec& AbilitySpec)
{
	if(ProtelumAbilitySystem.IsValid() && bCharacterStartAbilitiesGiven)
	{
		if(!IsGivenAbilityOwnable(AbilitySpec))
		{
			if(GetLocalRole() == ROLE_Authority)
			{
				UE_LOG(LogTemp, Warning , TEXT("%s %s() Ability is not owned"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__));
				ProtelumAbilitySystem->ClearAbility(AbilitySpec.Handle);
			}
			return;
		}
		
		if(GetLocalRole() == ROLE_Authority)
		{
			if(AbilitySpec.Ability->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag("Ability.AbilityClasses.Ultimate")))
			{
				SetAbilitySlotToModify(FGameplayTag::RequestGameplayTag("Ability.AbilitySlot.Dynamic.Ultimate"));
			}
			ModifyBoundAbilities(AbilitySpec);
		}
	}
	//Figure out solution so we dont call this if Ability is not Owned but not crashing with static abilities 
	if(IsLocallyControlled())
	{
		BindAbilityToInput(AbilitySpec);
	}
}

void AProtelumCharacterBase::UnbindAbility(FGameplayAbilitySpec& AbilitySpec)
{
	if(!ProtelumPlayerController.IsValid())
	{
		UE_LOG(LogTemp, Warning , TEXT("%s %s() PlayerController is null"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__));
		return;
	}
	ProtelumPlayerController->ClearInputBinding(AbilitySpec.Handle);
}

bool AProtelumCharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f; 
}

UAbilitySystemComponent* AProtelumCharacterBase::GetAbilitySystemComponent() const
{
	return ProtelumAbilitySystem.Get();
}

void AProtelumCharacterBase::OnMoveRequest(const FVector2D MoveValue)
{
	if(Controller != nullptr)
	{
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	
		if (MoveValue.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(MovementDirection, MoveValue.X);
		}

		if (MoveValue.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(MovementDirection, MoveValue.Y);
		}
	}
}

void AProtelumCharacterBase::OnLookRequest(const FVector2D LookValue)
{
	if(Controller != nullptr)
	{
		if (LookValue.X != 0.0f)
		{
			AddControllerYawInput(LookValue.X); // * LookRate
		}

		if (LookValue.Y != 0.0f)
		{
			AddControllerPitchInput(LookValue.Y); //* LookRate
		}
	}
}

void AProtelumCharacterBase::OnConfirmRequest() const
{
	if(ProtelumAbilitySystem.IsValid())
	{
		ProtelumAbilitySystem->LocalInputConfirm();
	}
}

void AProtelumCharacterBase::OnCancelRequest() const
{
	if(ProtelumAbilitySystem.IsValid())
	{
		ProtelumAbilitySystem->LocalInputCancel();
	}
}

void AProtelumCharacterBase::OnRequestSetMoveSpeed(const float NewMoveSpeed) const
{
	const TWeakObjectPtr<UCharacterMovementComponent> MovementComponent = GetCharacterMovement();
	if(MovementComponent.IsValid())
	{
		MovementComponent->MaxWalkSpeed = NewMoveSpeed;
	}
}

float AProtelumCharacterBase::GetMoveSpeed() const
{
	if(CharacterAttributeSet.IsValid())
	{
		return CharacterAttributeSet->GetMoveSpeed();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetCharacterLevel() const
{
	if(CharacterAttributeSet.IsValid())
	{
		return CharacterAttributeSet->GetCharacterLevel();
	}
	return 1.0f;
}

float AProtelumCharacterBase::GetXP() const
{
	if(CharacterAttributeSet.IsValid())
	{
		return CharacterAttributeSet->GetXP();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetXPBounty() const
{
	if(CharacterAttributeSet.IsValid())
	{
		return CharacterAttributeSet->GetXPBounty();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetGold() const
{
	if(CharacterAttributeSet.IsValid())
	{
		return CharacterAttributeSet->GetGold();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetGoldBounty() const
{
	if(CharacterAttributeSet.IsValid())
	{
		return CharacterAttributeSet->GetGoldBounty();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetHealth() const
{
	if(HealthAttributeSet.IsValid())
	{
		return HealthAttributeSet->GetHealth();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetMaxHealth() const
{
	if(HealthAttributeSet.IsValid())
	{
		return HealthAttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetHealthRegenerationRate() const
{
	if(HealthAttributeSet.IsValid())
	{
		return HealthAttributeSet->GetHealthRegenerationRate();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetShield() const
{
	if(HealthAttributeSet.IsValid())
	{
		return HealthAttributeSet->GetShield();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetDamageResistance() const
{
	if(HealthAttributeSet.IsValid())
	{
		return HealthAttributeSet->GetDamageResistance();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetShieldRegenerationRate() const
{
	if(HealthAttributeSet.IsValid())
	{
		return HealthAttributeSet->GetShieldRegenerationRate();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetCriticalHitResistance() const
{
	if(HealthAttributeSet.IsValid())
	{
		return HealthAttributeSet->GetCriticalHitResistance();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetDamageProjectileMaxAmmo() const
{
	if(ShamanWeaponAttributeSet.IsValid())
	{
		return ShamanWeaponAttributeSet->GetDamageProjectileMaxAmmo();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetDamageProjectileAmmo() const
{
	if(ShamanWeaponAttributeSet.IsValid())
	{
		return ShamanWeaponAttributeSet->GetDamageProjectileAmmo();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetDamageProjectileRegeneration() const
{
	if(ShamanWeaponAttributeSet.IsValid())
	{
		return ShamanWeaponAttributeSet->GetDamageProjectileRegeneration();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetProjectileDamage() const
{
	if(ShamanWeaponAttributeSet.IsValid())
	{
		return ShamanWeaponAttributeSet->GetProjectileDamage();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetHealProjectileMaxAmmo() const
{
	if(ShamanWeaponAttributeSet.IsValid())
	{
		return ShamanWeaponAttributeSet->GetHealProjectileMaxAmmo();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetHealProjectileAmmo() const
{
	if(ShamanWeaponAttributeSet.IsValid())
	{
		return ShamanWeaponAttributeSet->GetHealProjectileAmmo();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetHealProjectileRegeneration() const
{
	if(ShamanWeaponAttributeSet.IsValid())
	{
		return ShamanWeaponAttributeSet->GetDamageProjectileRegeneration();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetProjectileHeal() const
{
	if(ShamanWeaponAttributeSet.IsValid())
	{
		return ShamanWeaponAttributeSet->GetProjectileHeal();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetCriticalHitChance() const
{
	if(AttackerAttributeSet.IsValid())
	{
		return AttackerAttributeSet->GetCriticalHitChance();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetShieldPenetration() const
{
	if(AttackerAttributeSet.IsValid())
	{
		return AttackerAttributeSet->GetShieldPenetration();
	}
	return 0.0f;
}

float AProtelumCharacterBase::GetResistancePenetration() const
{
	if(AttackerAttributeSet.IsValid())
	{
		return AttackerAttributeSet->GetResistancePenetration();
	}
	return 0.0f;
}

void AProtelumCharacterBase::SetAbilitySlotToModify(FGameplayTag AbilitySlotToModify)
{
	TargetedAbilitySlot = AbilitySlotToModify;
}
FGameplayTag AProtelumCharacterBase::GetAbilitySlotToModify()
{
	const FGameplayTag GameplayTag = TargetedAbilitySlot;
	SetAbilitySlotToModify(FGameplayTag::EmptyTag);
	return GameplayTag;
}

void AProtelumCharacterBase::SetAbilityToReplace(FGameplayTag AbilityToReplace)
{
	bReplaceSpecificAbility = true;
	TargetedAbility = AbilityToReplace;
}

FGameplayTag AProtelumCharacterBase::GetAbilityToReplace()
{
	bReplaceSpecificAbility = false;
	const FGameplayTag GameplayTag = TargetedAbility;
	SetAbilitySlotToModify(FGameplayTag::EmptyTag);
	return GameplayTag;
}

bool AProtelumCharacterBase::IsGivenAbilityOwnable(const FGameplayAbilitySpec& AbilitySpec) const
{
	if(!IsValid(OwnableAbilityAsset))
	{
		UE_LOG(LogTemp, Error , TEXT("%s %s() Ability Libary is Empty, fill out BP"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__));
		return false;
	}
	
	for(auto Ability : OwnableAbilityAsset->GetOwnableAbilities())
	{
		if(Ability.GameplayAbilityClass.GetDefaultObject() == AbilitySpec.Ability)
		{
			return true;
		}
	}
	return false;
}

void AProtelumCharacterBase::ModifyBoundAbilities(const FGameplayAbilitySpec& AbilitySpec)
{
	//Modify BoundAbilities
	FGameplayTag SlotToFill = GetAbilitySlotToModify();
	if(SlotToFill == FGameplayTag::EmptyTag)
	{
		SlotToFill = FindAbilitySlotByDefaultObject(EmptyGameplayAbilityClass);
		if(SlotToFill == FGameplayTag::EmptyTag)
		{
			if(!bReplaceSpecificAbility)
			{
				UE_LOG(LogTemp, Error , TEXT("%s %s()  No slot to fill and no empty slot"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__));
				if(ProtelumAbilitySystem.IsValid())
				{
					ProtelumAbilitySystem->ClearAbility(AbilitySpec.Handle);
				}
				return;
			}
			const FGameplayTag OldAbilityTag = GetAbilityToReplace();
			if(OldAbilityTag == FGameplayTag::EmptyTag)
			{
				UE_LOG(LogTemp, Error , TEXT("%s %s() No Ability To Replace"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__));
				if(ProtelumAbilitySystem.IsValid())
				{
					ProtelumAbilitySystem->ClearAbility(AbilitySpec.Handle);
				}
				return;
			}
			const TSubclassOf<UProtelumGameplayAbility> OldAbilityClass = FindOwnableAbilityByTag(OldAbilityTag);
			SlotToFill = FindAbilitySlotByDefaultObject(OldAbilityClass);
			if(SlotToFill == FGameplayTag::EmptyTag)
			{
				UE_LOG(LogTemp, Error , TEXT("%s %s() There is no BoundAbility with such a GameplayTag"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__));
				if(ProtelumAbilitySystem.IsValid())
				{
					ProtelumAbilitySystem->ClearAbility(AbilitySpec.Handle);
				}
				return;
			}
		}
		else
		{
			//Making sure there is nothing Cached and bugs abilityGiving
			GetAbilityToReplace();
		}
	}
	FGameplayTag OldAbilitySlot = FGameplayTag::EmptyTag;
	bool bAbilityIsAlreadyBound = false;
	if(AbilitySpec.Ability != EmptyGameplayAbilityClass.GetDefaultObject())
	{
		OldAbilitySlot = IsAbilityAlreadyBound(AbilitySpec, bAbilityIsAlreadyBound);
	}
	
	const FGameplayAbilitySpecHandle OldAbilityHandle = ReplaceBoundAbilityBySlot(SlotToFill, AbilitySpec);;
	FGameplayAbilitySpec* TempOldAbilitySpec = FindAbilitySpecByHandle(OldAbilityHandle);
	FGameplayAbilitySpec OldAbilitySpec;
	bool bSpecIsValid = false;
	if(TempOldAbilitySpec)
	{
		OldAbilitySpec = *TempOldAbilitySpec;
		bSpecIsValid = true;
	}
	
	if(ProtelumAbilitySystem.IsValid())
	{
		ProtelumAbilitySystem->ClearAbility(OldAbilityHandle);
	}
	if(bAbilityIsAlreadyBound && OldAbilitySlot != SlotToFill && bSpecIsValid)
	{
		SetAbilitySlotToModify(OldAbilitySlot);
		ProtelumAbilitySystem->GiveAbility(FGameplayAbilitySpec(OldAbilitySpec.Ability));
	}
}



FGameplayTag AProtelumCharacterBase::FindAbilitySlotByDefaultObject(TSubclassOf<UProtelumGameplayAbility> AbilityToFind)
{
	for(const auto& Ability : BoundAbilities)
	{
		if(const FGameplayAbilitySpec* TempSpec = FindAbilitySpecByHandle(Ability.SpecHandle))
		{
			if(TempSpec->Ability == AbilityToFind.GetDefaultObject())
			{
				return Ability.SlotTag;
			}
		}		
	}
	return FGameplayTag::EmptyTag;
}

TSubclassOf<UProtelumGameplayAbility> AProtelumCharacterBase::FindOwnableAbilityByTag(FGameplayTag GameplayTag) const
{
	if(!IsValid(OwnableAbilityAsset))
	{
		UE_LOG(LogTemp, Error , TEXT("%s %s() Ability Libary is Empty, fill out BP"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__));
		return nullptr;
	}
	return OwnableAbilityAsset->GetOwnableAbilityClassByTag(&GameplayTag);

}

FGameplayAbilitySpec* AProtelumCharacterBase::FindAbilitySpecBySlotTag(const FGameplayTag SlotTag)
{
	for(auto& Ability : BoundAbilities)
	{
		if(Ability.SlotTag == SlotTag)
		{
			return FindAbilitySpecByHandle(Ability.SpecHandle);
		}
	}
	return nullptr;
}

FGameplayAbilitySpec* AProtelumCharacterBase::FindAbilitySpecByHandle(const FGameplayAbilitySpecHandle SpecHandle) const
{
	FGameplayAbilitySpec* TempSpec = nullptr;
	if(ProtelumAbilitySystem.IsValid())
	{
		TempSpec = ProtelumAbilitySystem->FindAbilitySpecFromHandle(SpecHandle);
	}
	return TempSpec;
}

void AProtelumCharacterBase::IncreaseAbilityLevelBySlot(const FGameplayTag& SlotToLevel)
{
	if(GetLocalRole() != ROLE_Authority)
	{
		return; //This is a test maybe it wont work
	}
	
	FGameplayAbilitySpec* Spec = FindAbilitySpecBySlotTag(SlotToLevel);
	if(Spec == nullptr)
	{
		UE_LOG(LogTemp, Error , TEXT("%s %s() Spec is null"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__));
		return;
	}
	int32 NewLevel = Spec->Level;
	NewLevel++;
	NewLevel = FMath::Clamp(NewLevel, 1, 5); //Todo: Make this in the Ability;
	if(IsValid(Spec->Ability))
	{
		const TObjectPtr<UProtelumGameplayAbility> Ability = Cast<UProtelumGameplayAbility>(Spec->Ability);
		if(IsValid(Ability) && Ability->ShouldAbilityBeRemovedOnLevelChange() && ProtelumAbilitySystem.IsValid())
		{
			SetAbilitySlotToModify(SlotToLevel);
			ProtelumAbilitySystem->GiveAbility(FGameplayAbilitySpec(Spec->Ability->StaticClass(), NewLevel, INDEX_NONE));
			return;
		}
	}
	FGameplayAbilitySpec* AbilitySpec = ProtelumAbilitySystem->FindAbilitySpecFromHandle(Spec->Handle);
	if(AbilitySpec != nullptr)
	{
		AbilitySpec->Level = NewLevel;
		ProtelumAbilitySystem->MarkAbilitySpecDirty(*AbilitySpec);
		Spec->Level = NewLevel;
	}
}

void AProtelumCharacterBase::DecreaseAbilityLevelBySlot(const FGameplayTag& SlotToLevel)
{
	if(GetLocalRole() != ROLE_Authority)
	{
		return; //This is a test maybe it wont work
	}
	FGameplayAbilitySpec* Spec = FindAbilitySpecBySlotTag(SlotToLevel);
	if(Spec == nullptr)
	{
		UE_LOG(LogTemp, Error , TEXT("%s %s() Spec is null"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__));
		return;
	}
	int32 NewLevel = Spec->Level;
	NewLevel--;
	NewLevel = FMath::Clamp(NewLevel, 1, 5); //Todo: Make this in the Ability;
	if(IsValid(Spec->Ability))
	{
		const TObjectPtr<UProtelumGameplayAbility> Ability = Cast<UProtelumGameplayAbility>(Spec->Ability);
		if(IsValid(Ability) && Ability->ShouldAbilityBeRemovedOnLevelChange() && ProtelumAbilitySystem.IsValid())
		{
			SetAbilitySlotToModify(SlotToLevel);
			ProtelumAbilitySystem->GiveAbility(FGameplayAbilitySpec(Spec->Ability->StaticClass(), NewLevel, INDEX_NONE));
			return;
		}
	}
	FGameplayAbilitySpec* AbilitySpec = ProtelumAbilitySystem->FindAbilitySpecFromHandle(Spec->Handle);
	if(AbilitySpec != nullptr)
	{
		AbilitySpec->Level = NewLevel;
		ProtelumAbilitySystem->MarkAbilitySpecDirty(*AbilitySpec);
		Spec->Level = NewLevel;
	}
}

FGameplayTag AProtelumCharacterBase::IsAbilityAlreadyBound(const FGameplayAbilitySpec& AbilitySpec,
                                                           bool& bFoundBoundAbility)
{
	for(const auto Ability : BoundAbilities)
	{
		if(const FGameplayAbilitySpec* TempSpec = FindAbilitySpecByHandle(Ability.SpecHandle))
		{
			if(TempSpec->Ability == AbilitySpec.Ability)
			{
				bFoundBoundAbility = true;
				return Ability.SlotTag;
			}
		}
	}
	return FGameplayTag::EmptyTag;
}

FGameplayAbilitySpecHandle AProtelumCharacterBase::ReplaceBoundAbilityBySlot(const FGameplayTag SlotToFill,
                                                                             const FGameplayAbilitySpec& AbilitySpec)
{
	FGameplayAbilitySpecHandle SpecHandleToRemove;
	for(auto& Ability : BoundAbilities)
	{
		if(Ability.SlotTag == SlotToFill)
		{
			SpecHandleToRemove = Ability.SpecHandle;
			Ability.GameplayAbilityClass = AbilitySpec.Ability->StaticClass();
			Ability.SpecHandle = AbilitySpec.Handle;
			break;
		}
	}
	return SpecHandleToRemove;
}

void AProtelumCharacterBase::BindAbilityToInput(const FGameplayAbilitySpec& AbilitySpecToBind)
{
	if(!ProtelumPlayerController.IsValid())
	{
		//Todo: Maybe interface or PawnComponent
		UE_LOG(LogTemp, Warning , TEXT("%s %s() PlayerController is null"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__));
		return;
	}

	for (const FGameplayAbilityInfo& BindInfo : BoundAbilities)
	{
		// if(BindInfo.GameplayAbilityClass == Spec.Ability.GetClass()) useful for replacing empty ability slots
		if(BindInfo.SpecHandle == AbilitySpecToBind.Handle)
		{
			ProtelumPlayerController->SetInputBinding(BindInfo.InputAction.LoadSynchronous(), AbilitySpecToBind.Handle);
			break;
		}
	}
}

void AProtelumCharacterBase::InitializeDefaultAttributes()
{
	if(!ProtelumAbilitySystem.IsValid())
	{
		UE_LOG(LogTemp, Warning , TEXT("%s() AbilitySystem is Null"),*FString(__FUNCTION__));
		return;
	}
	if(DefaultAttributeInitializeEffects.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in AbilitySystemComponent Blueprint of the Owner."), *FString(__FUNCTION__), *GetName());
		return;
	}
	FGameplayEffectContextHandle EffectContextHandle = ProtelumAbilitySystem->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	for(const TSubclassOf<UGameplayEffect> AttributesToAdd : DefaultAttributeInitializeEffects)
	{
		const FGameplayEffectSpecHandle TempOutSpecHandle = ProtelumAbilitySystem->MakeOutgoingSpec(AttributesToAdd, 1, EffectContextHandle);
		if(TempOutSpecHandle.IsValid())
		{
			// FActiveGameplayEffectHandle ActiveGEHandle =
			ProtelumAbilitySystem->ApplyGameplayEffectSpecToTarget(*TempOutSpecHandle.Data.Get(), ProtelumAbilitySystem.Get());
		}
	}
}

void AProtelumCharacterBase::ApplyStartupEffects()
{
	if(!ProtelumAbilitySystem.IsValid())
	{
		UE_LOG(LogTemp, Warning , TEXT("%s() AbilitySystem is Null"),*FString(__FUNCTION__));
		return;
	}
	if(StartupEffects.IsEmpty())
	{
		// UE_LOG(LogTemp, Error, TEXT("%s() Missing StartupEffects for %s. Please fill in AbilitySystemComponent Blueprint of the Owner."), *FString(__FUNCTION__), *GetName());
		return;
	}
	FGameplayEffectContextHandle EffectContextHandle = ProtelumAbilitySystem->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	for(const TSubclassOf<UGameplayEffect> GameplayEffectsToApply : StartupEffects)
	{
		const FGameplayEffectSpecHandle TempOutSpecHandle = ProtelumAbilitySystem->MakeOutgoingSpec(GameplayEffectsToApply, GetCharacterLevel(), EffectContextHandle);
		if(TempOutSpecHandle.IsValid())
		{
			// FActiveGameplayEffectHandle ActiveGEHandle =
			ProtelumAbilitySystem->ApplyGameplayEffectSpecToTarget(*TempOutSpecHandle.Data.Get(), ProtelumAbilitySystem.Get());
		}
	}
}

void AProtelumCharacterBase::LoadBoundAbilities()
{
	if(bBoundAbilitiesLoaded)
	{
		return;
	}
	//here we can check if we have a save file and then use that instead
	if(!IsValid(AbilityBindingAsset))
	{
		UE_LOG(LogTemp, Error , TEXT("%s %s() Ability Container is Empty, fill out BP"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__));
		return;
	}
	for(auto Ability : AbilityBindingAsset->GetAbilities())
	{
		BoundAbilities.Add(Ability);
	}
	EmptyGameplayAbilityClass = AbilityBindingAsset->GetEmptyAbilityClass();
	bBoundAbilitiesLoaded = true;
}

void AProtelumCharacterBase::LoadOwnableAbilities()
{
	if(bOwnableAbilitiesLoaded)
	{
		return;
	}
	if(!IsValid(OwnableAbilityAsset))
	{
		UE_LOG(LogTemp, Error , TEXT("%s %s() Ability Libary is Empty, fill out BP"), *UEnum::GetValueAsString(GetLocalRole()), *FString(__FUNCTION__));
		return;
	}
	OwnableAbilityClassTags = OwnableAbilityAsset->GetOwnableAbilitiesTags();
	bOwnableAbilitiesLoaded = true;
}

void AProtelumCharacterBase::AddCharacterStartAbilities()
{
	// Grant abilities, but only on the server
	//Todo: maybe do it like we do it with every other ability just grant and add to BoundAbilities on the Fly
	if (GetLocalRole() != ROLE_Authority || !ProtelumAbilitySystem.IsValid() || bCharacterStartAbilitiesGiven)
	{
		return;
	}
	if(!BoundAbilities.IsEmpty())
	{
		for(auto& AbilityToAdd : BoundAbilities)
		{
			FGameplayAbilitySpec TempSpec = FGameplayAbilitySpec(AbilityToAdd.GameplayAbilityClass, GetCharacterLevel(), INDEX_NONE);
			AbilityToAdd.SpecHandle = TempSpec.Handle;
			ProtelumAbilitySystem->GiveAbility(TempSpec);
		}
	}
	AddBaseAttackAbilities();
	AddAlwaysActiveAbilities();
	AddAbilitiesWithoutInputBindingToLoadOnStartup();
	bCharacterStartAbilitiesGiven = true;
}

void AProtelumCharacterBase::AddBaseAttackAbilities()
{
	for(auto& AbilityToAdd : StartupBaseAttackAbilities)
	{
		ProtelumAbilitySystem->GiveAbility(FGameplayAbilitySpec(AbilityToAdd, GetCharacterLevel(), INDEX_NONE));
	}
}

void AProtelumCharacterBase::AddAlwaysActiveAbilities()
{
	for(auto& AbilityToAdd : AlwaysActiveAbilities)
	{
		ProtelumAbilitySystem->GiveAbility(FGameplayAbilitySpec(AbilityToAdd, GetCharacterLevel(), INDEX_NONE));
	}
}

void AProtelumCharacterBase::AddAbilitiesWithoutInputBindingToLoadOnStartup()
{
	for(auto& AbilityToAdd : AbilitiesWithoutInputBindingToLoadOnStartup)
	{
		ProtelumAbilitySystem->GiveAbility(FGameplayAbilitySpec(AbilityToAdd, GetCharacterLevel(), INDEX_NONE));
	}
}
