// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityActors/DeployActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/ProtelumAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"
#include "AbilitySystem/AttributeSets/StaticActorAttributeSet.h"

// Sets default values
ADeployActor::ADeployActor() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	if(IsValid(Mesh))
	{
		SetRootComponent(Mesh);
	}	
	ASC = CreateDefaultSubobject<UProtelumAbilitySystemComponent>("AbilitySystem");
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	bReplicates = true;

	StaticActorAttributeSet = CreateDefaultSubobject<UStaticActorAttributeSet>(TEXT("StaticAttributeSet"));
	HealthAttributeSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"));
}

// Called when the game starts or when spawned
void ADeployActor::BeginPlay()
{
	Super::BeginPlay();
	
	if(IsValid(Mesh))
	{
		const TWeakObjectPtr<UMaterialInterface> MeshMaterial = Mesh->GetMaterial(0);
		DynamicMeshMaterial = UMaterialInstanceDynamic::Create(MeshMaterial.Get(), nullptr);
		Mesh->SetMaterial(0, DynamicMeshMaterial);
	}
	
	if(IsValid(ASC))
	{
		ASC->InitAbilityActorInfo(this, this);

		//Begin Binding Delegates On CharacterAttributeValueChanged
		HealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).AddUObject(this, &ADeployActor::HealthChanged);
		MaxHealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &ADeployActor::MaxHealthChanged);
		HealthRegenerationRateChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthRegenerationRateAttribute()).AddUObject(this, &ADeployActor::HealthRegenerationRateChanged);
		ShieldChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetShieldAttribute()).AddUObject(this, &ADeployActor::ShieldChanged);
		DamageResistanceDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetDamageResistanceAttribute()).AddUObject(this, &ADeployActor::DamageResistanceChanged);
		CriticalHitResistanceChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetCriticalHitResistanceAttribute()).AddUObject(this, &ADeployActor::CriticalHitResistanceChanged);
		ShieldRegenerationRateChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetShieldRegenerationRateAttribute()).AddUObject(this, &ADeployActor::ShieldRegenerationRateChanged);
		
		//End Binding Delegates On CharacterAttributeValueChanged
		
		//Begin Binding Delegates On AttributeValueChanged
		LifeTimeChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(StaticActorAttributeSet->GetLifeTimeAttribute()).AddUObject(this, &ADeployActor::LifeTimeChanged);
		//End Binding Delegates On AttributeValueChanged
	}
	
}

void ADeployActor::HealthChanged(const FOnAttributeChangeData& Data)
{
	// UE_LOG(LogTemp, Warning , TEXT("WallHealth %f"), Data.NewValue);
	if(Data.NewValue <= 0.0f)
	{
		Destroy();
	}
	if(GetMaxHealth() <= 0.0f)
	{
		return;
	}
	const float HealthPercentage = Data.NewValue / GetMaxHealth();
	if(IsValid(DynamicMeshMaterial))
	{
		DynamicMeshMaterial->SetScalarParameterValue(TEXT("Damage"), 1 - HealthPercentage);
	}
}

void ADeployActor::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	const float HealthPercentage = GetHealth() / Data.NewValue;
	if(IsValid(DynamicMeshMaterial))
	{
		DynamicMeshMaterial->SetScalarParameterValue(TEXT("Damage"),1 - HealthPercentage);
	}
}
void ADeployActor::HealthRegenerationRateChanged(const FOnAttributeChangeData& Data)
{
}

void ADeployActor::ShieldChanged(const FOnAttributeChangeData& Data)
{
}

void ADeployActor::DamageResistanceChanged(const FOnAttributeChangeData& Data)
{
}

void ADeployActor::CriticalHitResistanceChanged(const FOnAttributeChangeData& Data)
{
}

void ADeployActor::ShieldRegenerationRateChanged(const FOnAttributeChangeData& Data)
{
}

void ADeployActor::LifeTimeChanged(const FOnAttributeChangeData& Data)
{
	FTimerManager& Manager = GetWorldTimerManager();
	if(Manager.IsTimerActive(DestroyHandle))
	{
		Manager.ClearTimer(DestroyHandle);
	}
	GetWorldTimerManager().SetTimer(DestroyHandle, this, &ADeployActor::OnLifeTimeEnd, Data.NewValue);
}

// Called every frame
void ADeployActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ADeployActor::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float ADeployActor::GetHealth() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetHealth();
	}
	return 0.0f;
}

float ADeployActor::GetMaxHealth() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

float ADeployActor::GetHealthRegenerationRate() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetHealthRegenerationRate();
	}
	return 0.0f;
}

float ADeployActor::GetShield() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetShield();
	}
	return 0.0f;
}

float ADeployActor::GetDamageResistance() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetDamageResistance();
	}
	return 0.0f;
}

float ADeployActor::GetShieldRegenerationRate() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetShieldRegenerationRate();
	}
	return 0.0f;
}

float ADeployActor::GetCriticalHitResistance() const
{
	if(IsValid(HealthAttributeSet))
	{
		return HealthAttributeSet->GetCriticalHitResistance();
	}
	return 0.0f;
}

float ADeployActor::GetLifeTime() const
{
	if(IsValid(StaticActorAttributeSet))
	{
		return StaticActorAttributeSet->GetLifeTime();
	}
	return 0.0f;
}

UAbilitySystemComponent* ADeployActor::GetAbilitySystemComponent() const
{
	return ASC;
}

void ADeployActor::OnLifeTimeEnd()
{
	Destroy();
}

