// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CharacterAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"

UCLASS()
class SURVIVALDEMIOS_API ATPSCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();

	virtual void PostInitializeComponents() override;

	// IAbilitySystemInterface implementation
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; }

	//void AddWeaponToInventory(class AWeapon* Weapon);
	void SetWeaponController(class AWeapon* Weapon);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable)
	void Sidewalk(float Value);

	UFUNCTION(BlueprintCallable)
	void VerticalLook(float Value);

	UFUNCTION(BlueprintCallable)
	void HorizontalLook(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CharacterCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArmCamera;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AWeapon> BP_Rifle;

	UPROPERTY(BlueprintReadOnly, Category = "Jump")
	bool bIsJumping;

	class UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bIsDead;

	void StartJump();

	void StopJump();

	void CrouchToggle();

	void CrouchHold();

	void StandUp();

	void Death();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float Health = 100.f;

	AWeapon* CurrentWeapon;

	UPROPERTY()
	UInputComponent* CurrentInputComponent;

	UPROPERTY()
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY()
	UCharacterAttributeSet* AttributeSet;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	UFUNCTION()
	float GetHealth() const { return Health; }

	UFUNCTION()
	bool GetIsDead() const { return bIsDead; }
};
