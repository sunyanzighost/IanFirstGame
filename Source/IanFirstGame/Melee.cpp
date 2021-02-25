// Fill out your copyright notice in the Description page of Project Settings.


#include "Melee.h"

#include "GameplayTagsManager.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

// Default constructor
UMelee::UMelee()
{
	// Set its own tags, and also the tags to be cancelled
	AbilityTags.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Melee")));
	CancelAbilitiesWithTag.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability")));
}



// BeginPlay() of the ability
void UMelee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AbilityHandle = Handle;
	AbilityActorInfo = ActorInfo;
	AbilityActivationInfo = ActivationInfo;
	
	// Commits resources/cooldowns etc. ActivateAbility() must call this!
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	// Play the montage
	if(MeleeMontage)
	{
		int32 Skill = FMath::RandRange(0, MeleeMontage->CompositeSections.Num() - 1);

		// Create a montage proxy
		UAbilityTask_PlayMontageAndWait* MontageProxy = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "None", MeleeMontage, 1.f, MeleeMontage->CompositeSections[Skill].SectionName);
		MontageProxy->Activate();

		// Call the end ability on different stage of the montage_play
		MontageProxy->OnCompleted.AddDynamic(this, &UMelee::AbilityFinish);
		MontageProxy->OnBlendOut.AddDynamic(this, &UMelee::AbilityFinish);
		MontageProxy->OnCancelled.AddDynamic(this, &UMelee::AbilityFinish);
		MontageProxy->OnInterrupted.AddDynamic(this, &UMelee::AbilityFinish);
	}
}

// To be called to end the ability
void UMelee::AbilityFinish()
{
	EndAbility(AbilityHandle, AbilityActorInfo, AbilityActivationInfo, false, false);
}