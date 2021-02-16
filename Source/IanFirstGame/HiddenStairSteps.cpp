// Fill out your copyright notice in the Description page of Project Settings.


#include "HiddenStairSteps.h"
#include "HiddenStair.h"

// Sets default values
AHiddenStairSteps::AHiddenStairSteps()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Number of hidden step needed
	NumberOfStairSteps = 5;

	CurrentStairStepNumber = 0;
	
	// Variables for setting a random location for the next stair
	MinY = 350.f;
	MaxY = 400.f;

	MinZ = 150.f;
	MaxZ = 250.f;

	// WaitTime before spawning the next stair steps
	HiddenStairStepsWaitTime = 1.5f;
}

// Called when the game starts or when spawned
void AHiddenStairSteps::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(HiddenStairTimer, this, &AHiddenStairSteps::SpawnStairs, HiddenStairStepsWaitTime);	
}

// Called every frame
void AHiddenStairSteps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Spawn the stairs
void AHiddenStairSteps::SpawnStairs()
{
	// Set a random location for the next stair
	DeltaY = FMath::RandRange(MinY, MaxY);
	DeltaZ = FMath::RandRange(MinZ, MaxZ);

	DeltaVector = FVector(0.f, DeltaY, DeltaZ);

	// Spawn the stair	
	AHiddenStair* HiddenStair = GetWorld()->SpawnActor<AHiddenStair>(SingleHiddenStep, StartLocation, StartRotation);
	HiddenStair->RaiseStair();
	
	StartLocation -= DeltaVector;

	CurrentStairStepNumber++;

	// Spawn the remaining stairs
	if(CurrentStairStepNumber < NumberOfStairSteps)
	{
		GetWorldTimerManager().SetTimer(HiddenStairTimer, this, &AHiddenStairSteps::SpawnStairs, HiddenStairStepsWaitTime);
	}	
}

