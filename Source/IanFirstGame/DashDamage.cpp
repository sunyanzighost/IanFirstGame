// Fill out your copyright notice in the Description page of Project Settings.


#include "DashDamage.h"

#include "EnemyAttributeSet.h"
#include "GameplayTagsManager.h"

UDashDamage::UDashDamage()
{
	// Default values
	Damage = -10.f;

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
	FGameplayTag Tag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Dash.Damage"));
	
	InheritableGameplayEffectTags.AddTag(Tag);
}
