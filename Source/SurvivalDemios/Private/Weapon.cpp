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

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh"));
	MuzzleFlashEffect = CreateDefaultSubobject<UParticleSystemComponent>(FName("MuzzleFlashEffect"));
	BulletTrail = CreateDefaultSubobject<UParticleSystemComponent>(FName("BulletTrail"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/Rifle.Rifle'"));

	ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleFlashAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Effects/ParticleSystems/Weapons/AssaultRifle/Muzzle/P_AssaultRifle_MF.P_AssaultRifle_MF'"));

	ConstructorHelpers::FObjectFinder<UParticleSystem> BulletTrailAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Effects/ParticleSystems/Weapons/AssaultRifle/Muzzle/P_AssaultRifle_Trail.P_AssaultRifle_Trail'"));

	ConstructorHelpers::FObjectFinder<UParticleSystem> BulletImpactAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Effects/ParticleSystems/Weapons/AssaultRifle/Impacts/P_AssaultRifle_IH.P_AssaultRifle_IH'"));

	//ConstructorHelpers::FObjectFinder<UParticleSystem> BloodImpactAsset(TEXT(""));

	if (WeaponMeshAsset.Succeeded() && MuzzleFlashAsset.Succeeded() && BulletTrailAsset.Succeeded() && BulletImpactAsset.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(WeaponMeshAsset.Object);
		MuzzleFlashEffect->SetTemplate(MuzzleFlashAsset.Object);
		BulletTrail->SetTemplate(BulletTrailAsset.Object);
		BulletImpactFX = BulletImpactAsset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("One or more FObjectFinders failed to retrieve an asset"));
	}

	RootComponent = WeaponMesh;

	MuzzleArrow = CreateDefaultSubobject<UArrowComponent>(FName("Muzzle Arrow"));
	MuzzleArrow->SetupAttachment(WeaponMesh, FName("MuzzleFlashSocket"));
	MuzzleArrow->SetRelativeLocation(FVector(-4.f, 0, -1.2f));
	MuzzleArrow->SetRelativeScale3D(FVector(0.3f, 0.7f, 0.7f));

	MuzzleFlashEffect->SetupAttachment(MuzzleArrow);
	MuzzleFlashEffect->bAutoActivate = false;
	MuzzleFlashEffect->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));

	BulletTrail->SetupAttachment(MuzzleArrow);
	BulletTrail->bAutoActivate = false;
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
		UParticleSystemComponent* HitFX =
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				BulletImpactFX,
				HitResult.Location,
				HitResult.ImpactNormal.Rotation()
			);
	}
}
