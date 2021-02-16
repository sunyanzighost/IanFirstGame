// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyAIController.h"
#include "EnemySpawnBox.generated.h"

UCLASS()
class IANFIRSTGAME_API AEnemySpawnBox : public AActor
{
	GENERATED_BODY()

private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* SpawnBox;

	// Behavior tree
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* AIBehaviorTree;

	// Class of the enemy to be spawned
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AEnemy> Enemy;

	// Reference to the enemy to be spawned
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	AEnemy* EnemySpawned;

	// AI Controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEnemyAIController> EnemyAIController;

public:	
	// Sets default values for this actor's properties
	AEnemySpawnBox();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Get a random point inside the spawn box
	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomSpawnPoint();

	// Spawn the enemy
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spawning")
	void SpawnEnemy(UClass* EnemyClass, const FVector &SpawnLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
