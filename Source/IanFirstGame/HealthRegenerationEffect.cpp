// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthRegenerationEffect.h"
#include "GameplayTagsManager.h"
#include "MainCharacterAttributeSet.h"

UHealthRegenerationEffect::UHealthRegenerationEffect()
{
	// Default values
	HealthRegenerated = 25.f;
	
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo HealthModifier;

	FGameplayAttribute Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UMainCharacterAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMainCharacterAttributeSet, Health)));
	
	HealthModifier.Attribute = Attribute;

	HealthModifier.ModifierOp = EGameplayModOp::Additive;

	HealthModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(HealthRegenerated);
	
	Modifiers.Add(HealthModifier);

	// Add a gameplay tag
	FGameplayTag Tag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.HealthRegeneration.HealthEffect"));
	
	InheritableGameplayEffectTags.AddTag(Tag);

	// Add a gameplay cue tag
	FGameplayTag HealthRegenerationGameplayCueTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.HealthRegeneration.GameplayCue"));
	
	GameplayCues.Add(FGameplayEffectCue(HealthRegenerationGameplayCueTag, 0.f, 0.f));
}
