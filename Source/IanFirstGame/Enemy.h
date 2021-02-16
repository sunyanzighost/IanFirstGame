// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class IANFIRSTGAME_API AEnemy : public ACharacter
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

	// Ref. to the AIController
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controllers", meta = (AllowPrivateAccess = "true"))
	class AEnemyAIController* AIController;

	// Combat montage ref.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* CombatMontage;

	// Death montage ref.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;

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

	// Interp speed to face enemy
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float InterpToPlayerSpeed;

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
	void Die();

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
