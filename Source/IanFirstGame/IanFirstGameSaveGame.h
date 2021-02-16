// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "IanFirstGameSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FPlayerCharacterStats
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = "Save Game Data")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, Category = "Save Game Data")
	float Health;
	UPROPERTY(VisibleAnywhere, Category = "Save Game Data")
	float MaxStamina;
	UPROPERTY(VisibleAnywhere, Category = "Save Game Data")
	float Stamina;
	UPROPERTY(VisibleAnywhere, Category = "Save Game Data")
	int32 MaxSoul;
	UPROPERTY(VisibleAnywhere, Category = "Save Game Data")
	int32 Soul;
	UPROPERTY(VisibleAnywhere, Category = "Save Game Data")
	FVector PlayerLocation;
	UPROPERTY(VisibleAnywhere, Category = "Save Game Data")
	FRotator PlayerRotation;
	UPROPERTY(VisibleAnywhere, Category = "Save Game Data")
	FString WeaponName;
	UPROPERTY(VisibleAnywhere, Category = "Save Game Data")
	FString LevelName;
};


UCLASS()
class IANFIRSTGAME_API UIanFirstGameSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// Default constructor
	UIanFirstGameSaveGame();
	
	// PlayerCharacterStats struct instance
	UPROPERTY(VisibleAnywhere, Category = "Save Game Data")
	FString SlotName;
	UPROPERTY(VisibleAnywhere, Category = "Save Game Data")
	uint32 UserIndex;
	UPROPERTY(VisibleAnywhere, Category = "Save Game Data")
	FPlayerCharacterStats PlayerCharacterStats;
};
