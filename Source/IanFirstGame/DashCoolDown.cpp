// Fill out your copyright notice in the Description page of Project Settings.


#include "DashCoolDown.h"

#include "GameplayTagsManager.h"

UDashCoolDown::UDashCoolDown()
{
	// Default values
	CoolDownDuration = 5.f;

	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	DurationMagnitude = FGameplayEffectModifierMagnitude (FScalableFloat(CoolDownDuration));

	// Add a granted tag
	InheritableOwnedTagsContainer.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Dash.CoolDown")));
}
