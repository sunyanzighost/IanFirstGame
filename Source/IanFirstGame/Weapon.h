// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API AWeapon : public AItem
{
	GENERATED_BODY()

private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CombatBox;

	// Weapon swing sounds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	class USoundCue* SwingSound;

	// Weapon hitting enemy sounds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	USoundCue* EnemyHitSound;

	// Combat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	AController* WeaponInstigator;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Switch", meta = (AllowPrivateAccess = "true"))
	class AActor* Switch1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Switch", meta = (AllowPrivateAccess = "true"))
	class AActor* Switch2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Switch", meta = (AllowPrivateAccess = "true"))
	class AActor* Switch3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Switch", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AHiddenStairSteps> HiddenStairSteps;

	TArray<AActor*> SwitchArray;

	// Whether it should lower the sword switch or not
	bool bLowerSwordSwitch;

	// Whether it has already spawn the hidden stair steps or not
	bool bAlreadySpawnHiddenStairSteps;

public:
	// Sets default values for this actor's properties
	AWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// On begin overlap for the collision volume
	virtual void OnCollisionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// When the sword hits enemy
	UFUNCTION()
	void CombatBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Equip the weapon to the main character
	void Equip(class AMainCharacter* Player);

	// Play swing sound
	void PlaySwingSound();

	// Set the instigator for this weapon
	FORCEINLINE void SetInstigator(AController* Controller) { WeaponInstigator = Controller; }

	// Get the weapon name
	FORCEINLINE FString GetWeaponName() { return WeaponName; }

	// Lower the Switches
	void LowerSwitches(float DeltaTime);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
