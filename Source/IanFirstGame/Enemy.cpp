// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "MainCharacter.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainPlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "EnemyAIController.h"
#include "EnemyAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(RootComponent);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(RootComponent);

	WeaponBox1 = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox1"));
	WeaponBox1->SetupAttachment(GetMesh(), FName("AttackSocket1"));
	WeaponBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox2"));
	WeaponBox2->SetupAttachment(GetMesh(), FName("AttackSocket2"));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	// Set up ability system component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Ability system component"));

	// Set up attribute set component
	AttributeSetComponent = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("AttributeSet Component"));

	// Set the collision for the weapons response only to player pawn
	WeaponBox1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox1->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponBox1->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WeaponBox2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponBox2->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Setup default values
	AcceptanceDistance = 30.f;

	AttackMaxWaitTime = 3.f;
	AttackMinWaitTime = 1.f;
	
	MoveToWaitTime = 1.5f;

	MaxHealth = AttributeSetComponent->Health.GetCurrentValue();
	Health = MaxHealth;
	
	DestroyWaitTime = 2.f;

	Damage = 30.f;

	InterpToPlayerSpeed = 10.f;

	SoulGiven = 100;

	bIsDead = false;

	OriginalWalkSpeed = 150.f;
	MaximumWalkSpeed = 300.f;

	// Set the default walk speed
	GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Get hold of the AIController
	AIController = Cast<AEnemyAIController>(GetController());
	
	// Bind the functions to call when overlap events happened
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectionSphereBeginOverlap);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectionSphereEndOverlap);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereBeginOverlap);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereEndOverlap);

	WeaponBox1->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::WeaponBox1BeginOverlap);
	WeaponBox2->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::WeaponBox2BeginOverlap);

	AttributeSetComponent->OnHealthChange.AddDynamic(this, &AEnemy::OnHealthChange);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerTarget && !bAttacking) // Interp to face the player when there is a valid player target and not in the action of attacking
	{
		FRotator TargetRotation = GetLookAtPlayerTargetYawRotation(PlayerTarget);
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, InterpToPlayerSpeed);
		SetActorRotation(NewRotation);
	}

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Move to target player character
void AEnemy::MoveTo(AMainCharacter* Target)
{
	if (!AIController) return; // Early return if no ref.

	if (bAttacking) return; // Early return if still in the attacking action

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceDistance);

	AIController->MoveTo(MoveRequest);
}

// When player enter the detection sphere of the enemy 
void AEnemy::DetectionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(OtherActor))
	{
		bDetectionSphereOverlapping = true;

		PlayerTargetForMovingTo = PlayerCharacter;
		
		// Speed-up
		GetCharacterMovement()->MaxWalkSpeed = MaximumWalkSpeed;

		MoveTo(PlayerCharacter); // Move to the player

		if (DetectionSound) // Play the detection sound
		{
			UGameplayStatics::PlaySound2D(this, DetectionSound);
		}

		// Stop the AI logic in Behavior Tree
		if(AIController && AIController->GetBrainComponent())
		{
			AIController->GetBrainComponent()->StopLogic("DetectedPlayer");
		}
	}
}

// When player leaves the detection sphere of the enemy 
void AEnemy::DetectionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(OtherActor))
	{
		bDetectionSphereOverlapping = false;

		PlayerTargetForMovingTo = nullptr;

		GetWorldTimerManager().ClearTimer(MoveToTimer);

		// Slow down
		GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;

		if (AIController)
		{
			// Stop its current movement
			AIController->StopMovement();

			// Restart the AI logic in Behavior Tree 
			if (AIController->GetBrainComponent())
			{
				AIController->GetBrainComponent()->RestartLogic();
			}
		}
	}
}

void AEnemy::CombatSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(OtherActor))
	{
		bCombatSphereOverlapping = true;

		PlayerTarget = PlayerCharacter; // Set the player as its target

		PlayerCharacter->SetClosestEnemyAsTarget();

		Attack(); // Attack the player
	}
}

void AEnemy::CombatSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(OtherActor))
	{
		bCombatSphereOverlapping = false;

		PlayerTarget = nullptr; // Set player target as null when end overlapping the combat sphere

		// Clear the enemy target for the player for this particular enemy
		if (PlayerCharacter->GetEnemyTarget() == this)
		{
			PlayerCharacter->SetEnemyTarget(nullptr);

			PlayerCharacter->SetClosestEnemyAsTarget(); // Set the remaining closest one to be the target

			PlayerCharacter->GetMainPlayerController()->HideEnemyHealthBar();
		}

		if (!bIsStun)
		{
			// Clear the timer set for next attack
			GetWorldTimerManager().ClearTimer(AttackTimer);

			// Bind a function using delegate
			MoveToDelegate.BindUFunction(this, FName("MoveTo"), PlayerCharacter);

			if (bDetectionSphereOverlapping) // Move to player if the player is still inside the detection sphere
			{
				GetWorldTimerManager().SetTimer(MoveToTimer, MoveToDelegate, MoveToWaitTime, false);
			}
		}
	}
}

void AEnemy::WeaponBox1BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(OtherActor))
	{
		if (bAttacking) // Only deal damage during attacking
		{
			if (!DamageType || !AIController) return; // Early return if no ref.
			UGameplayStatics::ApplyDamage(PlayerCharacter, Damage, AIController, this, DamageType);
			UE_LOG(LogTemp, Warning, TEXT("Weapon1 dealing damage!"));
		}
	}
}

void AEnemy::WeaponBox2BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(OtherActor))
	{
		if (bAttacking) // Only deal damage during attacking
		{
			if (!DamageType || !AIController) return; // Early return if no ref.
			UGameplayStatics::ApplyDamage(PlayerCharacter, Damage, AIController, this, DamageType);
			UE_LOG(LogTemp, Warning, TEXT("Weapon2 dealing damage!"));
		}
	}
}

// Get the yaw rotation in the direction of the player to the enemy target
FRotator AEnemy::GetLookAtPlayerTargetYawRotation(AMainCharacter* Target)
{
	FRotator LookAtPlayerTargetYawRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	return FRotator(0.f, LookAtPlayerTargetYawRotation.Yaw, 0.f);
}

void AEnemy::Attack()
{
	// Stop the current movement before starting attacking
	if (AIController)
	{
		AIController->StopMovement();
	}

	UAnimInstance* EnemyAnimInstance = GetMesh()->GetAnimInstance();

	if (!EnemyAnimInstance || !CombatMontage) return; // Early return if no ref.

	bAttacking = true;

	int32 Skill = FMath::RandRange(0, 2);

	switch (Skill)
	{
	case 0:
		EnemyAnimInstance->Montage_Play(CombatMontage, 1.0f);
		EnemyAnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
		break;
	case 1:
		EnemyAnimInstance->Montage_Play(CombatMontage, 1.0f);
		EnemyAnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
		break;
	case 2:
		EnemyAnimInstance->Montage_Play(CombatMontage, 1.0f);
		EnemyAnimInstance->Montage_JumpToSection(FName("Attack_3"), CombatMontage);
		break;
	default:
		;
	}
}

// When end attacking
void AEnemy::AttackEnd()
{
	bAttacking = false;

	if (PlayerTarget)
	{
		if (PlayerTarget->GetPlayerStatus() == EPlayerStatus::EPS_Dead) return; // Do not have to perform next attack if the player is dead
	}

	// Perform next attack, after the AttackWaitTime, while the player is still inside the CombatSphere
	if (bCombatSphereOverlapping)
	{
		float AttackWaitTime = FMath::RandRange(AttackMinWaitTime, AttackMaxWaitTime);

		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackWaitTime);
	}
}

void AEnemy::ActivateCollision()
{
	WeaponBox1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponBox2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactivateCollision()
{
	WeaponBox1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	if (Health <= 0.f)
	{
		Die();

		// Give soul to the player when dead
		AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(DamageCauser->GetOwner());
		if (PlayerCharacter)
		{
			PlayerCharacter->IncrementSoul(SoulGiven);
			PlayerCharacter->SetClosestEnemyAsTarget();
		}

	}

	return DamageAmount;
}

void AEnemy::Die()
{
	bIsDead = true;

	// Get ref. to animinstance
	UAnimInstance* EnemyAnimInstance = GetMesh()->GetAnimInstance();

	if (!EnemyAnimInstance || !DeathMontage) return; // Early return if no ref.

	int32 Selection = FMath::RandRange(0, 1);
	
	// Play death montage
	switch (Selection)
	{
	case 0:
		EnemyAnimInstance->Montage_Play(DeathMontage, 1.f);
		EnemyAnimInstance->Montage_JumpToSection(FName("Death_1"), DeathMontage);
		break;
	case 1:
		EnemyAnimInstance->Montage_Play(DeathMontage, 1.f);
		EnemyAnimInstance->Montage_JumpToSection(FName("Death_2"), DeathMontage);
		break;
	default:
		;
	}

	// Play death sound
	if (DeathSound)
	{
		UGameplayStatics::PlaySound2D(this, DeathSound);
	}

	// Disable collision for all components of this enemy
	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;

	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AEnemy::RemoveFromGame, DestroyWaitTime);
}

void AEnemy::RemoveFromGame()
{
	Destroy();
}

UAbilitySystemComponent* AEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// When the health attribute being changed
void AEnemy::OnHealthChange(float CurrentValue, float MaxValue)
{
	Health = CurrentValue;

	UE_LOG(LogTemp, Warning, TEXT("I am being attacked!"));
	
	if(CurrentValue <= 0.f)
	{
		Die();
	}
}

// Enable the AI movement
void AEnemy::EnableMovement()
{
	bIsStun = false;
	
	if(bIsDead) return; // Early return if already dead

	// Resume the montage
	UAnimInstance* EnemyAnimInstance = GetMesh()->GetAnimInstance();
	EnemyAnimInstance->Montage_Resume(ReactStunMontage);

	if (bCombatSphereOverlapping)
	{
		Attack();
	}
	else if (bDetectionSphereOverlapping && PlayerTargetForMovingTo)
	{
		MoveTo(PlayerTargetForMovingTo);
	}
	else
	{
		if (AIController)
		{
			// Restart the AI logic in Behavior Tree 
			if (AIController->GetBrainComponent())
			{
				AIController->GetBrainComponent()->RestartLogic();
			}
		}
	}
}

// Disable the AI movement
void AEnemy::DisableMovement()
{
	// Clear all existing timers
	GetWorldTimerManager().ClearTimer(MoveToTimer);
	GetWorldTimerManager().ClearTimer(AttackTimer);
	GetWorldTimerManager().ClearTimer(DestroyTimer);

	// Stop the enemy current movement
	if(AIController)
	{
		AIController->StopMovement();
	}
}

// Being stun by a certain amount of time
void AEnemy::BeingStun(float StunTime)
{
	bIsStun = true;

	bAttacking = false;
	
	DisableMovement();

	// Play ReactStun montage
	UAnimInstance* EnemyAnimInstance = GetMesh()->GetAnimInstance();

	if(!EnemyAnimInstance || !ReactStunMontage) return; // Early return if no ref.

	EnemyAnimInstance->Montage_Play(ReactStunMontage, 1.f);

	EnemyAnimInstance->Montage_Pause(ReactStunMontage);

	GetWorldTimerManager().SetTimer(BeingStunTimerHandle, this, &AEnemy::EnableMovement, StunTime, false);
}

