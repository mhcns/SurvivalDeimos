// Fill out your copyright notice in the Description page of Project Settings.


#include "BotCharacter.h"
#include "Engine/World.h"
#include "Weapon.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Shoot.h"

// Sets default values
ABotCharacter::ABotCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(FName("Ability System Component"));
	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(FName("Attribute Set"));

}

// Called when the game starts or when spawned
void ABotCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystem)
	{
		AbilitySystem->InitAbilityActorInfo(this, this);
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(BP_Rifle, FTransform(), Params);
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WeaponSocket"));

	FGameplayAbilitySpec Spec(UGA_Shoot::StaticClass(), 1, 0, CurrentWeapon);
	if (AbilitySystem && HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Giving ability to weapon"));
		CurrentWeapon->SetOwnerAbilitySystem(AbilitySystem);
		AbilitySystem->GiveAbility(Spec);
	}
}

// Called every frame
void ABotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABotCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABotCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= ActualDamage;
	if (Health <= 0.f)
	{
		Death();
	}
	return ActualDamage;
}

void ABotCharacter::Death()
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (Capsule)
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Health = 0.f;
	bIsDead = true;
	//Destroy();
}
