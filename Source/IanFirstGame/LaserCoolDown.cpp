// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserCoolDown.h"

#include "GameplayTagsManager.h"

ULaserCoolDown::ULaserCoolDown()
{
	// Default values
	CoolDownDuration = 2.f;

	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	DurationMagnitude = FGameplayEffectModifierMagnitude (FScalableFloat(CoolDownDuration));

	// Add a granted tag
	InheritableOwnedTagsContainer.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Laser.CoolDown")));
}
