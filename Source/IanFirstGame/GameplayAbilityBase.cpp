// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityBase.h"
#include "GameplayTagsManager.h"

// Default Constructor
UGameplayAbilityBase::UGameplayAbilityBase()
{
	AbilityTags.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability")));
	CancelAbilitiesWithTag.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability")));
}
