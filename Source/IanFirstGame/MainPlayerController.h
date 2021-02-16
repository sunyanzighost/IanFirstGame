// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	// Player health and stamina bar
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> HUDClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUD;

	// Enemy health bar
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> EnemyHealthBarClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	UUserWidget* EnemyHealthBar;

	// Pause menu
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PauseMenuClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	UUserWidget* PauseMenu;

	// Variable to store enemy location for displaying the enemy health bar
	FVector EnemyLocation;

	// Ref. to the enemy target for displaying the enemy health bar
	class AEnemy* EnemyTargetForHealthBar;

	float EnemyHalfHeight;
	float EnemyRadius;

	// Boolean for checking if the pause menu is showing
	bool bPauseMenu;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Set the enemy location variable
	FORCEINLINE void SetEnemyLocation(FVector Location) { EnemyLocation = Location; }

	// Set the enemy target ref.
	FORCEINLINE void SetEnemyTargetForHealthBar(AEnemy* Target) { EnemyTargetForHealthBar = Target; }

	// Show enemy health bar
	void ShowEnemyHealthBar();

	// Hide enemy health bar
	void HideEnemyHealthBar();

	// Show pause menu
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void ShowPauseMenu();

	// Hide Pause Menu
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void HidePauseMenu();

	// Toggle pause menu
	void TogglePauseMenu();

protected:
	virtual void BeginPlay() override;
};
