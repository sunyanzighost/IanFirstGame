// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_UpdateTargetLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

// Default constructor
UBTTask_UpdateTargetLocation::UBTTask_UpdateTargetLocation()
{
    NodeName = "Update Target Location";

    Radius = 2000.f;

    bHaveRandomReachablePoint = false;
}

// Start this task
EBTNodeResult::Type UBTTask_UpdateTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("SelfLocation"));

    FNavLocation TargetNavLocation;

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    
    bHaveRandomReachablePoint = NavSys->GetRandomReachablePointInRadius(Origin, Radius, TargetNavLocation);

    if(bHaveRandomReachablePoint)
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), TargetNavLocation.Location);
    }

    return EBTNodeResult::Succeeded;
    
}
