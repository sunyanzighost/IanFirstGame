// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityBase.h"
#include "HealthRegeneration.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API UHealthRegeneration : public UGameplayAbilityBase
{
	GENERATED_BODY()

private:
	// Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HealthRegenerationMontage;

	//Gameplay effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay effect", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> HealthRegenerationEffect;

	FGameplayAbilitySpecHandle AbilityHandle;
	const FGameplayAbilityActorInfo* AbilityActorInfo;
	FGameplayAbilityActivationInfo AbilityActivationInfo;

	// Gameplay effect level
	float GameplayEffectLevel;

public:
	// Default constructor
	UHealthRegeneration();

	// Deal damage function
	UFUNCTION()
    void AbilityTakeEffect();

	// To be called to end the ability
	UFUNCTION()
    void AbilityFinish();

protected:
	// BeginPlay() of the ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
