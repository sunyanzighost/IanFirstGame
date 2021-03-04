// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityBase.h"
#include "Melee.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API UMelee : public UGameplayAbilityBase
{
	GENERATED_BODY()

private:
	// Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MeleeMontage;

	// Gameplay effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay effect", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> MeleeEffect;

	FGameplayAbilitySpecHandle AbilityHandle;
	const FGameplayAbilityActorInfo* AbilityActorInfo;
    FGameplayAbilityActivationInfo AbilityActivationInfo;

	float GameplayEffectLevel;

	float MeleeStunTime;
	float MeleePushForce;
	
public:
	// Default Constructor
	UMelee();

	// Deal damage function
	UFUNCTION()
	void AbilityDamage(FGameplayEventData Payload);

	// To be called to end the ability
	UFUNCTION()
	void AbilityFinish();
	

protected:
	// BeginPlay() of the ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
