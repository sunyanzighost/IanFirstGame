// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserDamage.h"

#include "EnemyAttributeSet.h"
#include "GameplayTagsManager.h"

ULaserDamage::ULaserDamage()
{
	// Default values
	Damage = -3.f;

	// Duration policy
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// Configure the attribute modifier
	FGameplayModifierInfo HealthModifier;

	FGameplayAttribute Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UEnemyAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UEnemyAttributeSet, Health)));
	
	HealthModifier.Attribute = Attribute;

	HealthModifier.ModifierOp = EGameplayModOp::Additive;

	HealthModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(Damage);
	
	Modifiers.Add(HealthModifier);

	// Add a gameplay tag
	FGameplayTag Tag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Laser.Damage"));
	
	InheritableGameplayEffectTags.AddTag(Tag);
}
