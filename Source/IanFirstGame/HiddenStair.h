// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HiddenStair.generated.h"

UCLASS()
class IANFIRSTGAME_API AHiddenStair : public AActor
{
	GENERATED_BODY()

private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	class USoundCue* RockRaisingSound;

	// Variables for the interpolation
	FVector StartPoint;
	float DeltaZ;
	float InterpSpeed;

	// Whether the stair should start raising or not
	bool bRaiseStair;

public:	
	// Sets default values for this actor's properties
	AHiddenStair();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Raise the stair to the EndPoint
	void RaiseStair();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
