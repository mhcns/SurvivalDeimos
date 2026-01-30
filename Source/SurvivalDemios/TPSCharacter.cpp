// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "Weapon.h"
#include "Engine/EngineTypes.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmCamera = CreateDefaultSubobject<USpringArmComponent>(FName("Spring Arm Component"));
	SpringArmCamera->bUsePawnControlRotation = true;
	SpringArmCamera->SetupAttachment(RootComponent);

	CharacterCamera = CreateDefaultSubobject<UCameraComponent>(FName("CharacterCamera"));
	CharacterCamera->SetupAttachment(SpringArmCamera);

	AutoPossessPlayer = EAutoReceiveInput::Player0; // change it in case of multiplayer
}

void ATPSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	SpringArmCamera->TargetArmLength = 300.f;
	SpringArmCamera->SetRelativeLocation(FVector(0.f, 60.f, 50.f));

	CharacterMovement = GetCharacterMovement();
	CharacterMovement->JumpZVelocity = 600.f;
	CharacterMovement->GravityScale = 1.5f;
	CharacterMovement->SetCrouchedHalfHeight(70.f);

	SpringArmCamera->bEnableCameraLag = true;
	SpringArmCamera->CameraLagSpeed = 20.f;
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(BP_Rifle, FTransform(), Params);
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WeaponSocket"));
	SetWeaponController(CurrentWeapon);
}

void ATPSCharacter::CrouchToggle()
{
	if (IsCrouched())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ATPSCharacter::CrouchHold()
{
	Crouch();
}

void ATPSCharacter::StandUp()
{
	UnCrouch();
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	CurrentInputComponent = PlayerInputComponent;
	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Sidewalk", this, &ATPSCharacter::Sidewalk);
	PlayerInputComponent->BindAxis("MouseY", this, &ATPSCharacter::VerticalLook);
	PlayerInputComponent->BindAxis("MouseX", this, &ATPSCharacter::HorizontalLook);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ATPSCharacter::CrouchHold);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ATPSCharacter::StandUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATPSCharacter::StopJump);

	PlayerInputComponent->BindAction("CrouchToggle", IE_Pressed, this, &ATPSCharacter::CrouchToggle);
}

void ATPSCharacter::SetWeaponController(AWeapon* Weapon)
{
	if (!CurrentInputComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Input Component found in Character"));
		return;
	}

	if (CurrentWeapon && CurrentWeapon != Weapon)
		CurrentWeapon = Weapon;

	CurrentInputComponent->BindAction("Fire", IE_Pressed, CurrentWeapon, &AWeapon::Fire);
}


void ATPSCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value, true);
}

void ATPSCharacter::Sidewalk(float Value)
{
	AddMovementInput(GetActorRightVector(), Value, true);
}

void ATPSCharacter::VerticalLook(float Value)
{
	AddControllerPitchInput(-Value);
}

void ATPSCharacter::HorizontalLook(float Value)
{
	AddControllerYawInput(Value);
}

void ATPSCharacter::StartJump()
{
	bIsJumping = true;
	Jump();
}

void ATPSCharacter::StopJump()
{
	bIsJumping = false;
	StopJumping();
}

float ATPSCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= ActualDamage;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Health: %f"), Health));
	if (Health <= 0.f)
	{
		Destroy();
	}
	return ActualDamage;
}
