// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeDamageEffect.h"

#include "EnemyAttributeSet.h"
#include "GameplayTagsManager.h"

// Default constructor
UMeleeDamageEffect::UMeleeDamageEffect()
{
	// Default values
	Damage = -30.f;
	
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo HealthModifier;

	FGameplayAttribute Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UEnemyAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UEnemyAttributeSet, Health)));
	
	HealthModifier.Attribute = Attribute;

	HealthModifier.ModifierOp = EGameplayModOp::Additive;

	HealthModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(Damage);
	
	Modifiers.Add(HealthModifier);

	// Add a gameplay tag
	FGameplayTag Tag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Melee.Damage"));
	
	InheritableGameplayEffectTags.AddTag(Tag);
}
