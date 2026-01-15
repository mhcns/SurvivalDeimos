// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/Rifle.Rifle'"));

	if (WeaponMeshAsset.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(WeaponMeshAsset.Object);
	}

	RootComponent = WeaponMesh;

	MuzzleArrow = CreateDefaultSubobject<UArrowComponent>(FName("Muzzle Arrow"));
	MuzzleArrow->SetupAttachment(WeaponMesh, FName("MuzzleFlashSocket"));
	MuzzleArrow->SetRelativeLocation(FVector(-4.f, 0, -1.2f));
	MuzzleArrow->SetRelativeScale3D(FVector(0.3f, 0.7f, 0.7f));

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
		FVector MuzzleDirection = MuzzleArrow->GetComponentRotation().Vector();
		FVector EndLocation = MuzzleLocation + (MuzzleDirection * 1000.f);

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(GetOwner());
		QueryParams.bTraceComplex = true;

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, EndLocation, ECC_Visibility, QueryParams);

		if (bHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Hit"));
		}

		DrawDebugLine(GetWorld(), MuzzleLocation, EndLocation, FColor::Green, false, 1.f, 0, 1.f);
	}
}
