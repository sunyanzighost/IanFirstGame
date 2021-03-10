// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttributeSet.h"

#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"

// Default constructor
UEnemyAttributeSet::UEnemyAttributeSet()
    :Health(300.f),
    MaxHealth(300.f)
{
}

// Action to be done after attributes being changed by gameplay effect
void UEnemyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// If health is the attribute being altered by any game effect
	if(Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UEnemyAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UEnemyAttributeSet, Health)))
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue()));

		OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue(),Data.EffectSpec.GetEffectContext().GetInstigator());
	}
}