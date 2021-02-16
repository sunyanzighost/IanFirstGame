// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"

// Default constructor
AEnemyAIController::AEnemyAIController()
{
    PatrolDistance = 500.f;
}

// BeginPlay for "Placed Actors"
void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    // See if have valid Pawn
    if(GetPawn())
    {
        // Run the behavior tree
        if(AIBehaviorTree)
        {
            RunBehaviorTree(AIBehaviorTree);
        }
        else
        {
            return;
        }

        UBlackboardComponent* AIBlackboardComponent = GetBlackboardComponent();

        if(AIBlackboardComponent)
        {              
            FVector InitialLocation = GetPawn()->GetActorLocation();

            // Setting the blackboard keys
            AIBlackboardComponent->SetValueAsVector(TEXT("SelfLocation"), InitialLocation);
        }
    }
}

// OnPossess for "Spawned Actors"
void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Run the behavior tree
    if(AIBehaviorTree)
    {
        RunBehaviorTree(AIBehaviorTree);
    }
    else
    {
        return;
    }

    AIPawn = GetPawn();
  
    if(!AIPawn) return;

    UBlackboardComponent* AIBlackboardComponent = GetBlackboardComponent();

    if(AIBlackboardComponent && AIPawn)
    {              
        FVector InitialLocation = AIPawn->GetActorLocation();

        // Setting the blackboard keys
        AIBlackboardComponent->SetValueAsVector(TEXT("SelfLocation"), InitialLocation);
    }
}
