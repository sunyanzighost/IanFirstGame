// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnBox.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"
#include "Engine/World.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "EnemyAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AEnemySpawnBox::AEnemySpawnBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));

}

// Called when the game starts or when spawned
void AEnemySpawnBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawnBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Get a random point inside the spawn box
FVector AEnemySpawnBox::GetRandomSpawnPoint()
{
	FVector BoxOrigin = SpawnBox->GetComponentLocation();
	FVector BoxExtent = SpawnBox->GetScaledBoxExtent();
	return UKismetMathLibrary::RandomPointInBoundingBox(BoxOrigin, BoxExtent);
}

void AEnemySpawnBox::SpawnEnemy_Implementation(UClass* EnemyClass, const FVector& SpawnLocation)
{
	if (!EnemyClass) return; //Early return if no ref.

	UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), Enemy, AIBehaviorTree, SpawnLocation);
}

