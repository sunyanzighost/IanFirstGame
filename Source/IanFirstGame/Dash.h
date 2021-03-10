// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityBase.h"
#include "MainCharacter.h"

#include "Dash.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API UDash : public UGameplayAbilityBase
{
	GENERATED_BODY()

private:
	// Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DashMontage;

	// Ability stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability stats", meta = (AllowPrivateAccess = "true"))
	float DashForwardImpulseSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability stats", meta = (AllowPrivateAccess = "true"))
	float DashPushForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability stats", meta = (AllowPrivateAccess = "true"))
	float DashStunTime;

	// GameplayEffect
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DashEffect;

	FVector ForwardDashVector;
	float OriginalGroundFriction;

	FGameplayAbilitySpecHandle AbilityHandle;
	const FGameplayAbilityActorInfo* AbilityActorInfo;
	FGameplayAbilityActivationInfo AbilityActivationInfo;

	float GameplayEffectLevel;

	// PlayerCharacter ref.
	AMainCharacter* PlayerCharacter;

public:
	// Default constructor
	UDash();

	// Deal Damage
	UFUNCTION()
    void AbilityDamage(FGameplayEventData Payload);

	// End the ability
	UFUNCTION()
    void AbilityFinish();
	
protected:
	// BeginPlay() of the ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
