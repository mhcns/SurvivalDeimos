// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"
#include "Math/UnrealMathUtility.h"
#include "Components/AudioComponent.h"
#include "GA_Shoot.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh"));
	RootComponent = WeaponMesh;

	MuzzleFlashEffect = CreateDefaultSubobject<UParticleSystemComponent>(FName("MuzzleFlashEffect"));
	BulletTrail = CreateDefaultSubobject<UParticleSystemComponent>(FName("BulletTrail"));
	SoundComponent = CreateDefaultSubobject<UAudioComponent>(FName("Sound Component"));

	AttributeSet = CreateDefaultSubobject<UWeaponAttributeSet>(FName("Attribute Set"));

	MuzzleArrow = CreateDefaultSubobject<UArrowComponent>(FName("Muzzle Arrow"));
	MuzzleArrow->SetupAttachment(WeaponMesh, FName("MuzzleFlashSocket"));
	MuzzleArrow->SetRelativeLocation(FVector(-4.f, 0, -1.2f));
	MuzzleArrow->SetRelativeScale3D(FVector(0.3f, 0.7f, 0.7f));

	MuzzleFlashEffect->SetupAttachment(MuzzleArrow);
	MuzzleFlashEffect->bAutoActivate = false;
	MuzzleFlashEffect->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));

	BulletTrail->SetupAttachment(MuzzleArrow);
	BulletTrail->bAutoActivate = false;

	SoundComponent->SetupAttachment(MuzzleArrow);
	SoundComponent->bAutoActivate = false;
}

void AWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BulletTrail->SetTrailSourceData(FName("MuzzleFlashSocket"), FName("MuzzleFlashSocket2"), ETrailWidthMode_FromCentre, 2.f);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire()
{
	if (OwnerAbilitySystem && HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Fire"));
		OwnerAbilitySystem->TryActivateAbilityByClass(UGA_Shoot::StaticClass());
	}
}