// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeSurvivalDeimos.h"
#include "TPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void AGameModeSurvivalDeimos::BeginPlay()
{
	Super::BeginPlay();
	ATPSCharacter* Player = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerHUDClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

}