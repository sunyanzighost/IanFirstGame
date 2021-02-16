// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Like BeginPlay()
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "Animation Property")
	void UpdateAnimationProperty();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Property")
	float CharacterMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Property")
	APawn* EnemyPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Property")
	class AEnemy* EnemyCharacter;
};
