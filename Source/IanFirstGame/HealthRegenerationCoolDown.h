// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "HealthRegenerationCoolDown.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API UHealthRegenerationCoolDown : public UGameplayEffect
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Regeneration", meta = (AllowPrivateAccess = "true"))
	float CoolDownDuration;
	
public:
	// Default constructor
	UHealthRegenerationCoolDown();
};
