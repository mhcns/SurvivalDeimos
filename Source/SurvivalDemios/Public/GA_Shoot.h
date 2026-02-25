// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Weapon.h"
#include "GA_Shoot.generated.h"

/**
 *
 */
UCLASS()
class SURVIVALDEMIOS_API UGA_Shoot : public UGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	void CheckBulletHit(AWeapon* Weapon, FHitResult HitResult);
};
