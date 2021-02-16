// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Setup player health and stamina bar
	if (HUDClass)
	{
		HUD = CreateWidget(this, HUDClass);
		HUD->AddToViewport();
	}

	// Set up enemy health bar
	if (EnemyHealthBarClass)
	{
		EnemyHealthBar = CreateWidget(this, EnemyHealthBarClass);
		EnemyHealthBar->AddToViewport();
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}

	// Set up pause menu
	if (PauseMenuClass)
	{
		PauseMenu = CreateWidget(this, PauseMenuClass);
		PauseMenu->AddToViewport();
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
	}

	// Default values
	EnemyLocation = FVector(0.f);
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyTargetForHealthBar)
	{
		UCapsuleComponent* EnemyCapsule = EnemyTargetForHealthBar->GetCapsuleComponent();
		if (EnemyCapsule)
		{
			EnemyHalfHeight = EnemyCapsule->GetScaledCapsuleHalfHeight();
			EnemyRadius = EnemyCapsule->GetScaledCapsuleRadius();
		}

		FVector2D EnemyHealthBarPosition;
		FVector2D EnemyHealthBarSize(300.f, 5.f);

		ProjectWorldLocationToScreen(EnemyLocation, EnemyHealthBarPosition);

		EnemyHealthBarPosition.Y -= EnemyHalfHeight; // Move the health bar above the enemy's head
		EnemyHealthBarPosition.X -= EnemyRadius;

		if (EnemyHealthBar)
		{
			EnemyHealthBar->SetPositionInViewport(EnemyHealthBarPosition);

			EnemyHealthBar->SetDesiredSizeInViewport(EnemyHealthBarSize);
		}
	}
}

// Show enemy health bar
void AMainPlayerController::ShowEnemyHealthBar()
{
	if (!EnemyHealthBar) return; // Early return if there is no ref.
	EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
}

// Hide enemy health bar
void AMainPlayerController::HideEnemyHealthBar()
{
	if (!EnemyHealthBar) return; // Early return if there is no ref.
	EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
}

void AMainPlayerController::ShowPauseMenu_Implementation()
{
	if (!PauseMenu) return; // Early return if there is not ref.

	PauseMenu->SetVisibility(ESlateVisibility::Visible);
	bPauseMenu = true;

	// Show mouse cursor
	bShowMouseCursor = true;

	UGameplayStatics::SetGamePaused(this, true);
}

void AMainPlayerController::HidePauseMenu_Implementation()
{
	if (!PauseMenu) return; // Early return if there is not ref.

	PauseMenu->SetVisibility(ESlateVisibility::Hidden);
	bPauseMenu = false;

	// Hide mouse cursor
	bShowMouseCursor = false;

	UGameplayStatics::SetGamePaused(this, false);
}

void AMainPlayerController::TogglePauseMenu()
{
	if (!PauseMenu) return; // Early return if there is not ref.
	if (bPauseMenu)
	{
		HidePauseMenu();
	}
	else
	{
		ShowPauseMenu();
	}
}
