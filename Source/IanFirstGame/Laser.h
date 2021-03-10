// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityBase.h"

#include "Laser.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API ULaser : public UGameplayAbilityBase
{
	GENERATED_BODY()

private:
	// Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LaserMontage;

	// Ability stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability stats", meta = (AllowPrivateAccess = "true"))
	float LaserPushForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability stats", meta = (AllowPrivateAccess = "true"))
	float LaserStunTime;

	// GameplayEffect
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> LaserEffect;

	// LaserBeam actor
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> LaserBeamActor;

	AActor* SpawnedLaserBeam;

	FGameplayAbilitySpecHandle AbilityHandle;
	const FGameplayAbilityActorInfo* AbilityActorInfo;
	FGameplayAbilityActivationInfo AbilityActivationInfo;

	float GameplayEffectLevel;

	class AMainCharacter* PlayerCharacter;
	
public:
	// Default constructor
	ULaser();

protected:
	// BeginPlay of the ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:
	// Deal damage
	UFUNCTION()
	void AbilityDamage(FGameplayEventData Payload);

	// On AbilityEnd
	UFUNCTION()
	void AbilityEnd(UGameplayAbility* Ability);
};
