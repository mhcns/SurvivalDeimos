// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CharacterAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "BotCharacter.generated.h"

UCLASS()
class SURVIVALDEMIOS_API ABotCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABotCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AWeapon> BP_Rifle;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bIsDead;

	UFUNCTION()
	void Death();

	UPROPERTY()
	UAbilitySystemComponent* AbilitySystem;

public:

	UPROPERTY()
	UCharacterAttributeSet* AttributeSet;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class AWeapon* CurrentWeapon;

	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	float GetHealth() const { return Health; }

	UFUNCTION()
	bool GetIsDead() const { return bIsDead; }
};
