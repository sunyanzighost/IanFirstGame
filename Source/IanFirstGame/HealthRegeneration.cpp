// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthRegeneration.h"
#include "GameplayTagsManager.h"

UHealthRegeneration::UHealthRegeneration()
{
	AbilityTags.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.HealthRegeneration")));
	CancelAbilitiesWithTag.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability")));
}
