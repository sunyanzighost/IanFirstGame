// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserCost.h"


#include "GameplayTagsManager.h"
#include "MainCharacterAttributeSet.h"

ULaserCost::ULaserCost()
{
	// Default values
	ManaCost = -1.f;

	DurationPolicy = EGameplayEffectDurationType::Infinite;

	FGameplayModifierInfo ManaModifier;

	FGameplayAttribute Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UMainCharacterAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMainCharacterAttributeSet, Mana)));
	
	ManaModifier.Attribute = Attribute;

	ManaModifier.ModifierOp = EGameplayModOp::Additive;

	ManaModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(ManaCost);
	
	Modifiers.Add(ManaModifier);

	// Set the effect apply period
	Period.Value = 1.f;

	// Add a gameplay tag
	FGameplayTag Tag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Laser.Cost"));
	
	InheritableGameplayEffectTags.AddTag(Tag);	
}
