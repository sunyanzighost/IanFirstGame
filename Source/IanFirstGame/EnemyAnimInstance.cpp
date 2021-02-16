// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

// Like BeginPlay()
void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Get enemy pawn
	EnemyPawn = TryGetPawnOwner();

	// Then cast to AEnemy character
	EnemyCharacter = Cast<AEnemy>(EnemyPawn);
}

void UEnemyAnimInstance::UpdateAnimationProperty()
{
	if (!EnemyPawn) return; // Early return if no ref.

	FVector Speed = EnemyPawn->GetVelocity();

	FVector CleanedSpeed = FVector(Speed.X, Speed.Y, 0.0f);

	CharacterMovementSpeed = CleanedSpeed.Size();
}
