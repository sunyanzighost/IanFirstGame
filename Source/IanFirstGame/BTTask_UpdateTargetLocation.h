// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UpdateTargetLocation.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API UBTTask_UpdateTargetLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	float Radius;

	bool bHaveRandomReachablePoint;

public:
	// Default constructor
	UBTTask_UpdateTargetLocation();

	// Start this task
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
