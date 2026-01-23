// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* MuzzleArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponFX")
	class UParticleSystemComponent* MuzzleFlashEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponFX")
	class UParticleSystemComponent* BulletTrail;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponFX")
	class UParticleSystem* BulletImpactFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponFX")
	class UParticleSystem* BloodImpactFX;

	void CheckBulletHit(FHitResult HitResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Fire();

};
