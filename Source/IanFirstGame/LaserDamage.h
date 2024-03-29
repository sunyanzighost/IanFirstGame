// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "LaserDamage.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API ULaserDamage : public UGameplayEffect
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Laser", meta = (AllowPrivateAccess = "true"))
	float Damage;
	
public:
	// Default constructor
	ULaserDamage();
	
};
