// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HiddenStairSteps.generated.h"

UCLASS()
class IANFIRSTGAME_API AHiddenStairSteps : public AActor
{
	GENERATED_BODY()

private:
	// A Single Hidden Step ref.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hidden Stair Steps", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AHiddenStair> SingleHiddenStep;

	// Number of hidden step needed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hidden Stair Steps", meta = (AllowPrivateAccess = "true"))
	int32 NumberOfStairSteps;

	int32 CurrentStairStepNumber;

	// The starting location and rotation for the first step
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hidden Stair Steps", meta = (AllowPrivateAccess = "true"))
	FVector StartLocation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hidden Stair Steps", meta = (AllowPrivateAccess = "true"))
	FRotator StartRotation;

	// Variables for setting a random location for the next stair
	FVector DeltaVector;

	float DeltaY;
	float DeltaZ;

	float MinY;
	float MaxY;
	float MinZ;
	float MaxZ;

	// Timer for spawning the stair steps
	FTimerHandle HiddenStairTimer;
	float HiddenStairStepsWaitTime;
	
public:	
	// Sets default values for this actor's properties
	AHiddenStairSteps();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Spawn the stairs
	void SpawnStairs();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
