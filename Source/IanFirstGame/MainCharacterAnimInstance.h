// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Like BeginPlay()
	virtual void NativeInitializeAnimation() override;

	// Function to update the variables each tick
	UFUNCTION(BlueprintCallable, Category = "Animation Property")
	void UpdateAnimationProperty();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Property")
	float CharacterMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Property")
	float CharacterMovementDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Property")
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Property")
	APawn* PlayerPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Property")
	class AMainCharacter* PlayerCharacter;
};
