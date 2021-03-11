// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityBase.generated.h"

UENUM(BlueprintType)
enum class ECostType : uint8
{
	ECT_Health	UMETA(DisplayName = "Health"),
	ECT_Mana	UMETA(DisplayName = "Mana"),
	ECT_MAX		UMETA(DisplayName = "DefaultMax")
};

USTRUCT(BlueprintType)
struct FGameplayAbilityInfo
{
	GENERATED_BODY()

	// GameplayAbilityInfo
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilityInfo")
	float CoolDownDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilityInfo")
	float CostToCast;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilityInfo")
	ECostType AbilityCostType;

	// Default constructor
	FGameplayAbilityInfo();

	FGameplayAbilityInfo(float InCoolDownDuration, float InCostToCast, ECostType InAbilityCostType);
	
};

UCLASS()
class IANFIRSTGAME_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

private:
	class AMainCharacter* PlayerCharacter;

public:
	// Default Constructor
	UGameplayAbilityBase();

	// Configure the AbilityInfo for this ability
	FGameplayAbilityInfo GetAbilityInfo();

	// Determine if the player has enough attribute to cast the ability
	bool HasEnoughAttributeToCast();
};
