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
	if (MuzzleArrow)
	{
		FVector MuzzleLocation = MuzzleArrow->GetComponentLocation();
		FRotator MuzzleRotator = MuzzleArrow->GetComponentRotation();
		FVector MuzzleDirection = MuzzleRotator.Vector();
		FVector EndLocation = MuzzleLocation + (MuzzleDirection * 1000.f);
		float Distance = 1000.f;

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(GetOwner());
		QueryParams.bTraceComplex = true;

		if (SoundComponent->GetSound() != FireSound)
		{
			SoundComponent->SetSound(FireSound);
		}
		SoundComponent->Play();

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, EndLocation, ECC_Visibility, QueryParams);

		if (bHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Hit"));
			EndLocation = HitResult.Location;
			Distance = HitResult.Distance;
			CheckBulletHit(HitResult);
		}

		MuzzleFlashEffect->ActivateSystem();
		BulletTrail->SetVectorParameter("ShockBeamEnd", EndLocation);
		//Trail->SetFloatParameter("BeamLength", Distance);
		BulletTrail->ActivateSystem(); // depois fazer o tiro sair na mira da câmera

		//DrawDebugLine(GetWorld(), MuzzleLocation, EndLocation, FColor::Green, false, 1.f, 0, 1.f);
	}
}

void AWeapon::CheckBulletHit(FHitResult HitResult)
{
	if (HitResult.GetActor()->Tags.Contains(TEXT("Enemy")))
	{
		// Spawn blood effect;
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BulletImpactFX,
			HitResult.Location,
			HitResult.ImpactNormal.Rotation()
		);

		FVector DecalSize = FVector(FMath::RandRange(10.f, 20.f));
		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			BulletDecalMaterial,
			DecalSize,
			HitResult.Location,
			HitResult.ImpactNormal.Rotation()
		);

		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ImpactSound,
			HitResult.Location
		);
	}
}
