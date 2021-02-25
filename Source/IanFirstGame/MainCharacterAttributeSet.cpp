// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"

// Default constructor
UMainCharacterAttributeSet::UMainCharacterAttributeSet()
	:Health(100.f),
	MaxHealth(100.f),
	Mana(200.f),
	MaxMana(200.f)
{
}

// Action to be done after attributes being changed by gameplay effect
void UMainCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// If health is the attribute being altered by any game effect
	if(Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UMainCharacterAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMainCharacterAttributeSet, Health)))
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue()));

		OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
	}
}