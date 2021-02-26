// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "MeleeDamageEffect.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API UMeleeDamageEffect : public UGameplayEffect
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float Damage;
	
public:
	// Default constructor
	UMeleeDamageEffect();
};
