// Fill out your copyright notice in the Description page of Project Settings.


#include "Dash.h"


#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Enemy.h"

UDash::UDash()
{
	// Set its own tags, and also the tags to be cancelled
	AbilityTags.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Dash")));
	CancelAbilitiesWithTag.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability")));

	// Default values
	DashForwardImpulseSpeed = 5000.f;
	DashPushForce = 2000.f;
	DashStunTime = 5.f;

	GameplayEffectLevel = 1.f;
	
}

void UDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AbilityHandle = Handle;
	AbilityActorInfo = ActorInfo;
	AbilityActivationInfo = ActivationInfo;
	
	// Commits resources/cooldowns etc. ActivateAbility() must call this!
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	// Add impulse force to the owner
	if(AMainCharacter* Main = Cast<AMainCharacter>(GetOwningActorFromActorInfo()))
	{
		PlayerCharacter = Main;
		
		// Change the collision response to overlap
		PlayerCharacter->SetCapsuleCollisionResponseToPawn(ECollisionResponse::ECR_Overlap);
		
		// Store the original friction value
		OriginalGroundFriction = PlayerCharacter->GetCharacterMovement()->GroundFriction;

		// Set the friction to be 0
		PlayerCharacter->GetCharacterMovement()->GroundFriction = 0.f;

		// Calculate impulse direction with ability force applied
		FVector ImpulseDirection = PlayerCharacter->GetActorForwardVector() * DashForwardImpulseSpeed;
		
		// Add impulse vector
		PlayerCharacter->GetCharacterMovement()->AddImpulse(ImpulseDirection, true);
	}

	// Play the montage
	if(DashMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageProxy = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("None"), DashMontage);
		MontageProxy->Activate();

		// Bind functions to be called upon difference stage of the montage play
		MontageProxy->OnCompleted.AddDynamic(this, &UDash::AbilityFinish);
		MontageProxy->OnBlendOut.AddDynamic(this, &UDash::AbilityFinish);
		MontageProxy->OnInterrupted.AddDynamic(this, &UDash::AbilityFinish);
		MontageProxy->OnCancelled.AddDynamic(this, &UDash::AbilityFinish);
	}

	// Listen to specific gameplay event
	FGameplayTag DamageEventTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Dash.Damage"));

	UAbilityTask_WaitGameplayEvent* DamageEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, DamageEventTag);

	// Upon receive on the tag, call the damage function
	if (DamageEvent)
	{
		DamageEvent->EventReceived.AddDynamic(this, &UDash::AbilityDamage);
		DamageEvent->Activate();
	}
}

void UDash::AbilityDamage(FGameplayEventData Payload)
{	
	// Configure the TargetDataHandle for applying the gameplay effect
	AActor* EnemyTarget = const_cast<AActor*>(Payload.Target);
	FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(EnemyTarget);
	
	if(DashEffect)
	{
		UGameplayAbility::ApplyGameplayEffectToTarget(AbilityHandle, AbilityActorInfo, AbilityActivationInfo, TargetDataHandle, DashEffect, GameplayEffectLevel);
	}

	// Push and stun the enemy
	if (AEnemy* Enemy = Cast<AEnemy>(EnemyTarget))
	{
		FVector TargetLocation = Enemy->GetActorLocation();
		
		// Calculate the direction for the impulse
		FVector ImpulseDirection = TargetLocation - PlayerCharacter->GetActorLocation();

		ImpulseDirection.Normalize();

		PlayerCharacter->PushStunTarget(Enemy, ImpulseDirection, DashPushForce, DashStunTime);
	}
}

void UDash::AbilityFinish()
{
	// Set the collision response back to block
	PlayerCharacter->SetCapsuleCollisionResponseToPawn(ECollisionResponse::ECR_Block);

	// Set the ground friction back to its original value
	PlayerCharacter->GetCharacterMovement()->GroundFriction = OriginalGroundFriction;
	
	EndAbility(AbilityHandle, AbilityActorInfo, AbilityActivationInfo, false, false);
}