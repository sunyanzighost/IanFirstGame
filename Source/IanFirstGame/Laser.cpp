// Fill out your copyright notice in the Description page of Project Settings.


#include "Laser.h"


#include "AbilitySystemBlueprintLibrary.h"
#include "Enemy.h"
#include "GameplayTagsManager.h"
#include "LaserBeam.h"
#include "MainCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/SphereComponent.h"

ULaser::ULaser()
{
	// Set its own tags, and also the tags to be cancelled
	AbilityTags.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Laser")));
	CancelAbilitiesWithTag.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability")));
	
	// Default values
	LaserPushForce = 600.f;
	LaserStunTime = 0.4f;

	GameplayEffectLevel = 1.f;
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

	// Get hold of the player character
	PlayerCharacter = Cast<AMainCharacter>(GetOwningActorFromActorInfo());

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
	if(PlayerCharacter)
	{
		if(SpawnedLaserBeam)
		{
			FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
			
			SpawnedLaserBeam->AttachToComponent(PlayerCharacter->GetMesh(), AttachmentTransformRules, FName("LaserSocket"));

			// Important step: set PlayerCharacter to be the owner, so that the owner's ability will be able to received the damage event
			SpawnedLaserBeam->SetOwner(PlayerCharacter);

			// Toggle the camera control of the player
			PlayerCharacter->ToggleCameraControlRotation(true);
		}
	}

	// Listen to the damage event
	FGameplayTag DamageEventTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Laser.Damage"));

	UAbilityTask_WaitGameplayEvent* DamageEventProxy = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, DamageEventTag);

	if (DamageEventProxy)
	{
		DamageEventProxy->EventReceived.AddDynamic(this, &ULaser::AbilityDamage);
		DamageEventProxy->Activate();
	}

	this->OnGameplayAbilityEnded.AddUFunction(this, FName("AbilityEnd"), this);
}

// Deal damage
void ULaser::AbilityDamage(FGameplayEventData Payload)
{
	if (HasEnoughAttributeToCast())
	{
		// Apply the gameplay effect to the target enemies
		if(LaserEffect)
		{
			UGameplayAbility::ApplyGameplayEffectToTarget(AbilityHandle, AbilityActorInfo, AbilityActivationInfo, Payload.TargetData, LaserEffect, GameplayEffectLevel);
		}

		// Push and stun the enemies
		TArray<AActor*> TargetEnemiesArray = UAbilitySystemBlueprintLibrary::GetActorsFromTargetData(Payload.TargetData, 0);

		if(TargetEnemiesArray.Num() <= 0) return; // Early return if there is no target in the enemy array

		if(!PlayerCharacter) return; // Early return

		for(auto Actor : TargetEnemiesArray)
		{
			// Cast to AEnemy class
			AEnemy* Enemy = Cast<AEnemy>(Actor);

			// Calculate the impulse direction
			FVector ImpulseDirection;

			ALaserBeam* LaserBeam = Cast<ALaserBeam>(SpawnedLaserBeam);

			FVector EnemyLocation = Enemy->GetActorLocation();
			FVector LaserEndLocation = LaserBeam->GetLaserEndSphere()->GetComponentLocation();

			ImpulseDirection = EnemyLocation - LaserEndLocation;
			ImpulseDirection.Normalize();
		
			PlayerCharacter->PushStunTarget(Enemy, ImpulseDirection, LaserPushForce, LaserStunTime);
		}
	}
	else
	{
		EndAbility(AbilityHandle, AbilityActorInfo, AbilityActivationInfo, false, true);
	}
}

// On AbilityEnd
void ULaser::AbilityEnd(UGameplayAbility* Ability)
{
	// Remove the Laser cost effect
	FGameplayTag CostEffectTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Laser.Cost"));

	FGameplayTagContainer AssetTag;
	AssetTag.AddTag(CostEffectTag);
	
	UGameplayAbility::BP_RemoveGameplayEffectFromOwnerWithAssetTags(AssetTag);

	// Destroy the LaserBeam actor
	SpawnedLaserBeam->Destroy();

	// Toggle the player character camera rotation back to normal
	PlayerCharacter->ToggleCameraControlRotation(false);

	// Stop the montage
	MontageStop();

	UE_LOG(LogTemp, Warning, TEXT("not enough mana for Laser!"));
}