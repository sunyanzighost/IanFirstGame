// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class IANFIRSTGAME_API AEnemy : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DetectionSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CombatSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WeaponBox1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* WeaponBox2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	// AbilitySystem component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	// AttributeSet component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UEnemyAttributeSet* AttributeSetComponent;

	// Ref. to the AIController
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controllers", meta = (AllowPrivateAccess = "true"))
	class AEnemyAIController* AIController;

	// Combat montage ref.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* CombatMontage;

	// Death montage ref.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;

	// ReactStun montage ref.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReactStunMontage;

	// Sounds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	class USoundCue* DetectionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	class USoundCue* DeathSound;

	// Booleans for determining whether these components are being overlapped or not
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bCombatSphereOverlapping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bDetectionSphereOverlapping;

	// Combat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AcceptanceDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsStun;

	// Attack wait time
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AttackMaxWaitTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AttackMinWaitTime;
	FTimerHandle AttackTimer;

	// Move to player wait time
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MoveToWaitTime;
	FTimerHandle MoveToTimer;
	FTimerDelegate MoveToDelegate;

	// Destroy after death wait time
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float DestroyWaitTime;
	FTimerHandle DestroyTimer;

	// Enemy stats
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 SoulGiven;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float OriginalWalkSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MaximumWalkSpeed;

	// Reference to the main character
	class AMainCharacter* PlayerTarget;

	class AMainCharacter* PlayerTargetForMovingTo;

	// Interp speed to face enemy
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float InterpToPlayerSpeed;

	// Timer for being stunned
	FTimerHandle BeingStunTimerHandle;

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Move to target player character
	UFUNCTION()
	void MoveTo(class AMainCharacter* Target);

	// When player enter the detection sphere of the enemy 
	UFUNCTION()
	void DetectionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// When player leaves the detection sphere of the enemy 
	UFUNCTION()
	void DetectionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// When player enter the combat sphere of the enemy 
	UFUNCTION()
	void CombatSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// When player leaves the combat sphere of the enemy 
	UFUNCTION()
	void CombatSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// When its 1st weapon hits player
	UFUNCTION()
	void WeaponBox1BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// When its 2nd weapon hits player
	UFUNCTION()
	void WeaponBox2BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Get the yaw rotation in the direction of the player to the enemy target
	FRotator GetLookAtPlayerTargetYawRotation(AMainCharacter* Target);

	// Attack
	void Attack();

	// When end attacking
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AttackEnd();

	// Activate collision for the weapon box
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ActivateCollision();
	
	// Deactivate collision for the weapon box
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DeactivateCollision();

	// Taking damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Die
	void Die(AMainCharacter* PlayerCharacter);

	// When ending death animation
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DeathEnd();

	// Get the enemy removed from the game
	void RemoveFromGame();

	// For determining if it is dead or not
	UFUNCTION(BlueprintCallable, Category = "Combat")
	FORCEINLINE bool IsDead() { return bIsDead; }

	// Get current health
	FORCEINLINE float GetHealth() { return Health; }

	// Setters for the AIController
	FORCEINLINE void SetAIController(AEnemyAIController* NewController) { AIController = NewController; }

	// Getters for the AIController
	FORCEINLINE AEnemyAIController* GetAIController() { return AIController; }
	
	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// When the health attribute being changed
	UFUNCTION()
	void OnHealthChange(float CurrentValue, float MaxValue, AActor* InstigatorActor);

	// Enable the AI movement
	void EnableMovement();

	// Disable the AI movement
	void DisableMovement();

	// Being stun by a certain amount of time
	void BeingStun(float StunTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
