// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnimInstance.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Get player pawn
	PlayerPawn = TryGetPawnOwner();
	
	// Then cast it to player character
	PlayerCharacter = Cast<AMainCharacter>(PlayerPawn);
}

void UMainCharacterAnimInstance::UpdateAnimationProperty()
{
	if (!PlayerPawn) return; // early return if we don't have the pawn

	// Get the movement speed of the character
	FVector Speed = PlayerCharacter->GetVelocity();

	FVector CleanedSpeed(Speed.X, Speed.Y, 0.0f);

	CharacterMovementSpeed = CleanedSpeed.Size();


	// Boolean to check if the character is in air
	bIsInAir = PlayerCharacter->GetMovementComponent()->IsFalling();
}