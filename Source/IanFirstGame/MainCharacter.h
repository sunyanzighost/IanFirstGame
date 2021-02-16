// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	EPS_Normal			UMETA(DisplayName = "Normal"),
	EPS_Sprinting		UMETA(DisplayName = "Sprinting"),
	EPS_Dead			UMETA(DisplayName = "Dead"),
	EPS_MAX				UMETA(DisplayName = "DefaultMax") // This is not meant to be used, just a default value for the very last one
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal				UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum		UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted			UMETA(DisplayName = "Exhausted"),
	ESS_Recovery			UMETA(DisplayName = "Recovery"),
	ESS_MAX					UMETA(DisplayName = "DefaultMax")
};


UCLASS()
class IANFIRSTGAME_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	// Reference to the overlapping item
	class AItem* OverappingItem;

	// Referece to the current equipped weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class AWeapon* EquippedWeapon;

	// Reference to the main player controller
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class AMainPlayerController* MainPlayerController;

	// Reference to the enemy target
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class AEnemy* EnemyTarget;

	// Filter for getting overlapping actors
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> EnemyFilter;

	// Combat montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* CombatMontage;

	// Interp speed to face enemy
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float InterpToEnemySpeed;

	// Weapon storage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AWeaponStorage> WeaponStorage;

	// Player stats
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
	float MaxStamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
	float Stamina;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
	int32 MaxSoul;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
	int32 Soul;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
	float SprintingSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
	float RunningSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
	float StaminaRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
	float MinSprintRequriedStamina;

	// Whether the sprint key being pressed
	bool bSprintKeyDown;

	// Whether the player is attacking
	bool bAttacking;

	// Player status
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
	EPlayerStatus PlayerStatus;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
	EStaminaStatus StaminaStatus;

	// Sounds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	class USoundCue* DeathSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	USoundCue* PainSound;

	// Particles
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Particles", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* DeathParticle;

	// GameInstance variable
	class UIanFirstGame_GameInstance* GameInstance;

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Moving forward/ backward
	void MoveForward(float AxisValue);

	// Moving right/ left
	void MoveRight(float AxisValue);

	// Looking up/ down
	void LookUp(float AxisValue);

	// Looking right/ left
	void LookRight(float AxisValue);

	// Jump
	virtual void Jump() override;

	// Sprinting
	void Sprint();
	void StopSprint();

	// Setters for the overlapping item
	FORCEINLINE void SetOverlappingItem(AItem* Target) { OverappingItem = Target; }

	// Use item
	void UseItem();

	// Pause the game
	void PauseGame();

	// Get the main player controller
	FORCEINLINE AMainPlayerController* GetMainPlayerController() { return MainPlayerController; }

	// Set the enemy target
	FORCEINLINE void SetEnemyTarget(AEnemy* Target) { EnemyTarget = Target; }

	// Get the enemy target
	FORCEINLINE AEnemy* GetEnemyTarget() { return EnemyTarget; }

	// Set the closest enemy to be the enemy target when there are multiple enemies around the player
	void SetClosestEnemyAsTarget();

	// Get the yaw rotation in the direction of the player to the enemy target
	FRotator GetLookAtEnemyTargetYawRotation(AEnemy* Target);

	// Attack
	void Attack();

	// When end attacking
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AttackEnd();

	// Whether the player is attacking
	FORCEINLINE bool IsAttacking() { return bAttacking; }

	// Taking damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Die
	void Die();

	/** Called when the actor falls out of the world 'safely' (below KillZ and such) */
	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

	// Setters for the equipped weapon
	void SetEquippedWeapon(AWeapon* Weapon);

	// Set the player status
	void SetPlayerStatus(EPlayerStatus Status);

	// Get the player status
	FORCEINLINE EPlayerStatus GetPlayerStatus() { return PlayerStatus; }

	// Set the player stamina status
	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	// Increment health
	UFUNCTION(BlueprintCallable, Category =  "Pickup")
	void IncrementHealth(float Amount);

	// Increment soul
	void IncrementSoul(int32 Amount);

	// Saving game
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	// Saving game before switch level
	void SaveGameBeforeSwitchLevel(FString SlotName, int32 UserIndex);

	// Saving player status and weapon
	void SavePlayerStatusAndWeapon(class UIanFirstGameSaveGame* SaveGame);

	// Saving player location and level name
	void SavePlayerLocationAndLevelName(UIanFirstGameSaveGame* SaveGame);

	// Loading game
	UFUNCTION(BlueprintCallable)
	void LoadGame();

	// Loading game without switching level
	void LoadGameNoSwitchLevel();

	// Loading game after switching level
	void LoadGameAfterSwitchLevel(FString SlotName, int32 UserIndex);

	// Loading player status and weapon
	void LoadPlayerStatusAndWeapon(UIanFirstGameSaveGame* LoadGame);

	// Enable player controller and hide pause menu after load game
	void UpdateCharacterControlAfterLoadGame();

	// Quitting game
	UFUNCTION(BlueprintCallable)
	void QuitGame();

	// Switching level
	void SwitchLevel(FName LevelName);

	// Get current map name
	FString GetCurrentMapName();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	
};
