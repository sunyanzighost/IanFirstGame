// Fill out your copyright notice in the Description page of Project Settings.


#include "Melee.h"

#include "GameplayTagsManager.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Enemy.h"
#include "MainCharacter.h"

// Default constructor
UMelee::UMelee()
{
	// Set its own tags, and also the tags to be cancelled
	AbilityTags.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Melee")));
	CancelAbilitiesWithTag.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability")));

	// Default values
	GameplayEffectLevel = 1.f;

	MeleePushForce = 200.f;

	MeleeStunTime = 2.f;
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

	// Listen to the specific gameplay event of specific tag
	FGameplayTag DamageTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Melee.Damage"));

	UAbilityTask_WaitGameplayEvent* DamageEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, DamageTag);

	// Bind and trigger function to be called after the event is received
	if (DamageEvent)
	{
		DamageEvent->EventReceived.AddDynamic(this, &UMelee::AbilityDamage);
		DamageEvent->Activate();
	}
}

// Deal damage function
void UMelee::AbilityDamage(FGameplayEventData Payload)
{
	// Get the target from the Payload data, cast from const AActor* to AActor*
	AActor* EnemyTarget = const_cast<AActor*>(Payload.Target);

	// Make game data from the target actor, for the ApplyGameplayEffectToTarget function
	FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(EnemyTarget);	

	// Apply the damage effect to the target
	if(MeleeEffect)
	{
		UGameplayAbility::ApplyGameplayEffectToTarget(AbilityHandle, AbilityActorInfo, AbilityActivationInfo, TargetDataHandle, MeleeEffect, GameplayEffectLevel);
	}

	// Push and stun the target
	if(AEnemy* Enemy = Cast<AEnemy>(EnemyTarget))
	{
		// Calculate the impulse direction and normalize it
		FVector TargetLocation = Enemy->GetActorLocation();

		AMainCharacter* OwningCharacter = Cast<AMainCharacter>(GetOwningActorFromActorInfo());

		if(!OwningCharacter) return;

		FVector ImpulseDirection = TargetLocation - OwningCharacter->GetActorLocation();

		ImpulseDirection.Normalize();

		OwningCharacter->PushStunTarget(Enemy, ImpulseDirection, MeleePushForce, MeleeStunTime);
	}
}

// To be called to end the ability
void UMelee::AbilityFinish()
{
	EndAbility(AbilityHandle, AbilityActorInfo, AbilityActivationInfo, false, false);
}