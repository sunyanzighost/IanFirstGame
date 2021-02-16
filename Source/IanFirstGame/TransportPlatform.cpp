// Fill out your copyright notice in the Description page of Project Settings.


#include "TransportPlatform.h"
#include "TimerManager.h"

// Sets default values
ATransportPlatform::ATransportPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// Default values
	InterpSpeed = 0.5f;

	bInterp = false;

	InterpWaitTime = 3.f;
}

// Called when the game starts or when spawned
void ATransportPlatform::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the start/ end interppoint & calculate the distance between the 2 points
	StartInterpPoint = GetActorLocation();

	// Set the end point in terms of world' space, as it is currently the relative position from the start point
	EndInterpPoint += StartInterpPoint;

	Distance = FVector::Distance(StartInterpPoint, EndInterpPoint);

	// Set the timer for initial toggle
	GetWorldTimerManager().SetTimer(InterpTimer, this, &ATransportPlatform::ToggleInterp, InterpWaitTime);
}

// Called every frame
void ATransportPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Interp from start point to end point and then reverse after wait time
	if (bInterp)
	{
		FVector CurrentPoint = GetActorLocation();
		CurrentPoint = FMath::VInterpTo(CurrentPoint, EndInterpPoint, DeltaTime, InterpSpeed);
		SetActorLocation(CurrentPoint);

		float DistanceMoved = FVector::Distance(StartInterpPoint, CurrentPoint);

		if (Distance - DistanceMoved <= 5.f)
		{
			ToggleInterp();
			SwapStartEndInterpPoint(StartInterpPoint, EndInterpPoint);
			GetWorldTimerManager().SetTimer(InterpTimer, this, &ATransportPlatform::ToggleInterp, InterpWaitTime);
		}
	}
}

void ATransportPlatform::SwapStartEndInterpPoint(FVector& Start, FVector& End)
{
	FVector Temp = Start;
	Start = End;
	End = Temp;
}