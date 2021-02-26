// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeDamageEffect.h"

#include "EnemyAttributeSet.h"

// Default constructor
UMeleeDamageEffect::UMeleeDamageEffect()
{
	Damage = -30.f;
	
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo HealthModifier;

	FGameplayAttribute Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UEnemyAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UEnemyAttributeSet, Health)));
	
	HealthModifier.Attribute = Attribute;

	HealthModifier.ModifierOp = EGameplayModOp::Additive;

	HealthModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(Damage);
	
	Modifiers.Add(HealthModifier);
}
