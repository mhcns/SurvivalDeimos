// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Shoot.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

void UGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("FireAbility"));
	AWeapon* Weapon = Cast<AWeapon>(GetCurrentSourceObject());

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;


	if (Weapon->MuzzleArrow)
	{
		FVector MuzzleLocation = Weapon->MuzzleArrow->GetComponentLocation();
		FRotator MuzzleRotator = Weapon->MuzzleArrow->GetComponentRotation();
		FVector MuzzleDirection = MuzzleRotator.Vector();
		float Distance = Weapon->AttributeSet->Range.GetCurrentValue();
		FVector EndLocation = MuzzleLocation + (MuzzleDirection * Distance);

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Weapon);
		QueryParams.AddIgnoredActor(Weapon->GetOwner());
		QueryParams.bTraceComplex = true;

		if (Weapon->SoundComponent->GetSound() != Weapon->FireSound)
		{
			Weapon->SoundComponent->SetSound(Weapon->FireSound);
		}
		Weapon->SoundComponent->Play();

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, EndLocation, ECC_Visibility, QueryParams);

		if (bHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Hit"));
			EndLocation = HitResult.Location;
			Distance = HitResult.Distance;
			CheckBulletHit(Weapon, HitResult);
		}

		Weapon->MuzzleFlashEffect->ActivateSystem();
		Weapon->BulletTrail->SetVectorParameter("ShockBeamEnd", EndLocation);
		//Trail->SetFloatParameter("BeamLength", Distance);
		Weapon->BulletTrail->ActivateSystem(); // depois fazer o tiro sair na mira da câmera

		//DrawDebugLine(GetWorld(), MuzzleLocation, EndLocation, FColor::Green, false, 1.f, 0, 1.f);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Shoot::CheckBulletHit(AWeapon* Weapon, FHitResult HitResult)
{
	AActor* HitActor = HitResult.GetActor();

	if (!HitActor)
		return;

	if (HitActor->Tags.Contains(TEXT("Character")))
	{
		// Spawn blood effect;
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			Weapon->BloodImpactFX,
			HitResult.Location,
			HitResult.ImpactNormal.Rotation()
		);

		if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(HitActor))
		{
			UAbilitySystemComponent* TargetASC = ASCInterface->GetAbilitySystemComponent();
			UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

			FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
			ContextHandle.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle =
				SourceASC->MakeOutgoingSpec(Weapon->DamageEffect, 1.f, ContextHandle);

			if (SpecHandle.IsValid())
			{
				const UWeaponAttributeSet* WeaponAttributes =
					Weapon->AttributeSet;

				// Negative value because we want to apply damage, and in our AttributeSet we want to subtract the value from the health attribute
				float DamageValue = -WeaponAttributes->BaseDamage.GetCurrentValue();

				SpecHandle.Data->SetSetByCallerMagnitude(
					FGameplayTag::RequestGameplayTag("Data.Damage"),
					DamageValue
				);

				SourceASC->ApplyGameplayEffectSpecToTarget(
					*SpecHandle.Data.Get(),
					TargetASC
				);
			}
		}

		//UGameplayStatics::ApplyDamage(
		//	HitResult.GetActor(),
		//	20.f,
		//	GetInstigatorController(),
		//	this,
		//	nullptr
		//);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			Weapon->BulletImpactFX,
			HitResult.Location,
			HitResult.ImpactNormal.Rotation()
		);

		FVector DecalSize = FVector(FMath::RandRange(10.f, 20.f));
		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			Weapon->BulletDecalMaterial,
			DecalSize,
			HitResult.Location,
			HitResult.ImpactNormal.Rotation()
		);

		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			Weapon->ImpactSound,
			HitResult.Location
		);
	}
}
