// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameModeSurvivalDeimos.generated.h"

/**
 *
 */
UCLASS()
class SURVIVALDEMIOS_API AGameModeSurvivalDeimos : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Health")
	TSubclassOf<UUserWidget> PlayerHUDClass;

	UPROPERTY()
	class UUserWidget* CurrentWidget;
};
