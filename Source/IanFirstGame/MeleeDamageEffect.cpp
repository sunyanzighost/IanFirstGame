// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeDamageEffect.h"
#include "MainCharacterAttributeSet.h"

UMeleeDamageEffect::UMeleeDamageEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo HealthModifier;

	FGameplayAttribute Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UMainCharacterAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMainCharacterAttributeSet, Health)));
	
	HealthModifier.Attribute = Attribute;
	
	Modifiers.Add(HealthModifier);
}
