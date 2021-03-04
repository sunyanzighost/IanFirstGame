// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthRegeneration.h"
#include "GameplayTagsManager.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UHealthRegeneration::UHealthRegeneration()
{
	AbilityTags.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.HealthRegeneration")));
	CancelAbilitiesWithTag.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability")));

	// Default values
	GameplayEffectLevel = 1.f;
}

void UHealthRegeneration::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AbilityHandle = Handle;
	AbilityActorInfo = ActorInfo;
	AbilityActivationInfo = ActivationInfo;

	// Create the montage proxy and activate it
	if (HealthRegenerationMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageProxy = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "None", HealthRegenerationMontage);
		MontageProxy->Activate();
		
		MontageProxy->OnCompleted.AddDynamic(this, &UHealthRegeneration::AbilityTakeEffect);
		MontageProxy->OnBlendOut.AddDynamic(this, &UHealthRegeneration::AbilityTakeEffect);
		MontageProxy->OnCancelled.AddDynamic(this, &UHealthRegeneration::AbilityFinish);
		MontageProxy->OnInterrupted.AddDynamic(this, &UHealthRegeneration::AbilityFinish);
	}
}

void UHealthRegeneration::AbilityTakeEffect()
{
	// Commits resources/cooldowns etc. ActivateAbility() must call this!
	CommitAbility(AbilityHandle, AbilityActorInfo, AbilityActivationInfo);
	
	// Apply the gameplay effect
	if(HealthRegenerationEffect)
	{
		UGameplayAbility::ApplyGameplayEffectToOwner(AbilityHandle, AbilityActorInfo, AbilityActivationInfo, HealthRegenerationEffect.GetDefaultObject(), GameplayEffectLevel);
	}

	UE_LOG(LogTemp, Warning, TEXT("Doing health regeneration!"));

	// End the ability
	AbilityFinish();
}

void UHealthRegeneration::AbilityFinish()
{
	EndAbility(AbilityHandle, AbilityActorInfo, AbilityActivationInfo, false, false);
}