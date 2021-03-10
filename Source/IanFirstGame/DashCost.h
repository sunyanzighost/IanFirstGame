// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "DashCost.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API UDashCost : public UGameplayEffect
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float ManaCost;
	
public:
	// Default constructor
	UDashCost();
};
