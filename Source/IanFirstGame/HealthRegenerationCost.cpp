// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthRegenerationCost.h"


#include "GameplayTagsManager.h"
#include "MainCharacterAttributeSet.h"

UHealthRegenerationCost::UHealthRegenerationCost()
{
	// Default values
	ManaCost = -30.f;

	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ManaModifier;

	FGameplayAttribute Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UMainCharacterAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMainCharacterAttributeSet, Mana)));
	
	ManaModifier.Attribute = Attribute;

	ManaModifier.ModifierOp = EGameplayModOp::Additive;

	ManaModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(ManaCost);
	
	Modifiers.Add(ManaModifier);

	// Add a gameplay tag
	FGameplayTag Tag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.HealthRegeneration.Cost"));
	
	InheritableGameplayEffectTags.AddTag(Tag);	
}
