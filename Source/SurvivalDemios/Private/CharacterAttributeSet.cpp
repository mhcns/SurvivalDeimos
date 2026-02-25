// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include <Net/UnrealNetwork.h>


UCharacterAttributeSet::UCharacterAttributeSet()
{
	Health = FGameplayAttributeData(100.f);
	MaxHealth = FGameplayAttributeData(100.f);
}

void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);
}

void UCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Clamp health to [0, MaxHealth]
		float NewHealth = FMath::Clamp(GetHealth(), 0.f, MaxHealth.GetBaseValue());
		SetHealth(NewHealth);

		if (NewHealth <= 0.f)
		{
			// Handle character death here (e.g., trigger death animation, disable input, etc.)
			UE_LOG(LogTemp, Warning, TEXT("Character has died."));
		}
	}
}