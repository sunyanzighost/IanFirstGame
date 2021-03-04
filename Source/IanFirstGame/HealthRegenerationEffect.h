// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "HealthRegenerationEffect.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API UHealthRegenerationEffect : public UGameplayEffect
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Regeneration", meta = (AllowPrivateAccess = "true"))
	float HealthRegenerated;
	
public:
	// Default constructor
	UHealthRegenerationEffect();
};
