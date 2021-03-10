// Fill out your copyright notice in the Description page of Project Settings.


#include "Laser.h"

#include "GameplayTagsManager.h"
#include "MainCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"

ULaser::ULaser()
{
	// Set its own tags, and also the tags to be cancelled
	AbilityTags.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Laser")));
	CancelAbilitiesWithTag.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability")));
	
	// Default values
	LaserPushForce = 600.f;
	LaserStunTime = 0.4f;
}

void ULaser::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AbilityHandle = Handle;
	AbilityActorInfo = ActorInfo;
	AbilityActivationInfo = ActivationInfo;
	
	// Commits resources/cooldowns etc. ActivateAbility() must call this!
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	// Play the montage
	if(LaserMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageProxy = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("None"), LaserMontage, 1.f, FName("Start"));
		MontageProxy->Activate();
	}

	// Spawn the LaserBeam actor
	if(LaserBeamActor)
	{
		FGameplayAbilityTargetDataHandle LaserTargetDataHandle;
		
		UAbilityTask_SpawnActor* SpawnActorProxy = UAbilityTask_SpawnActor::SpawnActor(this,LaserTargetDataHandle, LaserBeamActor);
		SpawnActorProxy->BeginSpawningActor(this, LaserTargetDataHandle, LaserBeamActor, SpawnedLaserBeam);
		SpawnActorProxy->FinishSpawningActor(this, LaserTargetDataHandle, SpawnedLaserBeam);
	}

	// Attach the LaserBeam actor to the player
	if(AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(GetOwningActorFromActorInfo()))
	{
		if(SpawnedLaserBeam)
		{
			FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
			
			SpawnedLaserBeam->AttachToComponent(PlayerCharacter->GetMesh(), AttachmentTransformRules, FName("LaserSocket"));

			SpawnedLaserBeam->SetOwner(PlayerCharacter);

			// Toggle the camera control of the player
			PlayerCharacter->ToggleCameraControlRotation(true);
		}
	}
}
