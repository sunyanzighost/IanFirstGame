// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "EnemyAttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyAttributeChangeDelegate, float, CurrentValue, float, MaxValue);

UCLASS()
class IANFIRSTGAME_API UEnemyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	// Default constructor
	UEnemyAttributeSet();

	// Health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute Set")
	FGameplayAttributeData Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute Set")
	FGameplayAttributeData MaxHealth;

	// Action to be done after attributes being changed by gameplay effect
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	FOnEnemyAttributeChangeDelegate OnHealthChange;
};
