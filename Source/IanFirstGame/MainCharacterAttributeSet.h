// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MainCharacterAttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangeDelegate, float, CurrentValue, float, MaxValue);

UCLASS()
class IANFIRSTGAME_API UMainCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	// Default constructor
	UMainCharacterAttributeSet();

	// Health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute Set")
	FGameplayAttributeData Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute Set")
	FGameplayAttributeData MaxHealth;

	// Mana
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute Set")
	FGameplayAttributeData Mana;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute Set")
	FGameplayAttributeData MaxMana;

	// Action to be done after attributes being changed by gameplay effect
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	FOnAttributeChangeDelegate OnHealthChange;
};