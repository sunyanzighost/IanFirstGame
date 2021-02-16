// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TransportPlatform.generated.h"

UCLASS()
class IANFIRSTGAME_API ATransportPlatform : public AActor
{
	GENERATED_BODY()

private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	
	// Start point for interping
	UPROPERTY(VisibleAnywhere)
	FVector StartInterpPoint;
	
	// End point for interping
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
	FVector EndInterpPoint;

	// Distance between the start and end interppoint
	float Distance;

	// Speed for interping
	UPROPERTY(EditAnywhere)
	float InterpSpeed;

	// Boolean for controlling whether to interp or not
	bool bInterp;

	// Timer for interping
	float InterpWaitTime;
	FTimerHandle InterpTimer;

public:	
	// Sets default values for this actor's properties
	ATransportPlatform();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Toggle interping
	FORCEINLINE void ToggleInterp() { bInterp = !bInterp; }

	// Swapping the start and end interppoint
	void SwapStartEndInterpPoint(FVector &Start, FVector &End);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
