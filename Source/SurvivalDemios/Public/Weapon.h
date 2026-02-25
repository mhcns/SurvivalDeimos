// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "WeaponAttributeSet.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class SURVIVALDEMIOS_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	UAbilitySystemComponent* OwnerAbilitySystem;

	UPROPERTY()
	UWeaponAttributeSet* AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* MuzzleArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* SoundComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoundFX")
	class USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoundFX")
	class USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponFX")
	class UParticleSystemComponent* MuzzleFlashEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponFX")
	class UParticleSystemComponent* BulletTrail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponFX")
	class UParticleSystem* BulletImpactFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponFX")
	class UParticleSystem* BloodImpactFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponFX")
	class UMaterialInterface* BulletDecalMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DamageEffect")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UFUNCTION()
	void Fire();

	void SetOwnerAbilitySystem(UAbilitySystemComponent* NewOwnerAbilitySystem) { OwnerAbilitySystem = NewOwnerAbilitySystem; }

};
