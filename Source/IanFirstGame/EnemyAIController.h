// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

private:
	// Behavior tree
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* AIBehaviorTree;

	// How far does it patrol
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float PatrolDistance;

	APawn* AIPawn;

public:
	// Default constructor
	AEnemyAIController();

protected:
	// BeginPlay for "Placed Actors"
	virtual void BeginPlay() override;

	// OnPossess for "Spawned Actors"
	virtual void OnPossess(APawn* InPawn) override;
	
};
