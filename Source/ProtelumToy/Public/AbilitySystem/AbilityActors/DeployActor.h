// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "DeployActor.generated.h"

class UHealthAttributeSet;
class UProtelumAbilitySystemComponent;
class UStaticActorAttributeSet;
UCLASS()
class PROTELUMTOY_API ADeployActor : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeployActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Begin Health AttributeChangedDelegate
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle HealthRegenerationRateChangedDelegateHandle;
	FDelegateHandle ShieldChangedDelegateHandle;
	FDelegateHandle DamageResistanceDelegateHandle;
	FDelegateHandle CriticalHitResistanceChangedDelegateHandle;
	FDelegateHandle ShieldRegenerationRateChangedDelegateHandle;
	//End Health AttributeChangedDelegate

	//Begin StaticActor AttributeChangedDelegate
	FDelegateHandle LifeTimeChangedDelegateHandle;
	//End StaticActor AttributeChangedDelegate
	
	
	//Begin Health AttributeChangedCallbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void HealthRegenerationRateChanged(const FOnAttributeChangeData& Data);
	virtual void ShieldChanged(const FOnAttributeChangeData& Data);
	virtual void DamageResistanceChanged(const FOnAttributeChangeData& Data);
	virtual void CriticalHitResistanceChanged(const FOnAttributeChangeData& Data);
	virtual void ShieldRegenerationRateChanged(const FOnAttributeChangeData& Data);
	//End Health AttributeChangedCallbacks

	//Begin StaticActor AttributeChangedCallbacks
	virtual void LifeTimeChanged(const FOnAttributeChangeData& Data);
	//End StaticActor AttributeChangedCallbacks
	
	FTimerHandle DestroyHandle;
public:
	bool IsAlive() const;

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
	
	//Begin StaticActor Attribute Getter
	UFUNCTION(BlueprintCallable, Category = "Protelum|AbilitySystem|AttributeSet|Deployable")
	float GetLifeTime() const;
	//End StaticActor Attribute Getter

	//Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//End IAbilitySystemInterface
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProtelumAbilitySystemComponent> ASC = {nullptr};
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh = {nullptr};
		
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DefaultAttributes = {nullptr};
	
private:
	
	void OnLifeTimeEnd();
	//Material of the MeshRootComponent
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMeshMaterial = {nullptr};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticActorAttributeSet> StaticActorAttributeSet = {nullptr};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthAttributeSet> HealthAttributeSet = {nullptr};
};
