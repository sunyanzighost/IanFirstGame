// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "LaserCoolDown.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API ULaserCoolDown : public UGameplayEffect
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Laser", meta = (AllowPrivateAccess = "true"))
	float CoolDownDuration;
	
public:
	// Default constructor
	ULaserCoolDown();
};
