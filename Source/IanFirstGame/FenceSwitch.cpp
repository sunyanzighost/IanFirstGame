// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceSwitch.h"

#include "K2Node_DoOnceMultiInput.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFenceSwitch::AFenceSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	
	DeltaZ = 350.f;

	InterpSpeed = 0.5f;

	bSoundPlayed = false;
}

// Called when the game starts or when spawned
void AFenceSwitch::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = Mesh->GetComponentLocation();
	
}

// Called every frame
void AFenceSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Lower the fence
void AFenceSwitch::LowerFence(float DeltaTime)
{
	FVector CurrentPoint = GetActorLocation();
	FVector EndInterpPoint = InitialLocation;
	EndInterpPoint.Z -= DeltaZ;
	
	CurrentPoint = FMath::VInterpTo(CurrentPoint, EndInterpPoint, DeltaTime, InterpSpeed);
	SetActorLocation(CurrentPoint);

	// Play the sound
	if (!bSoundPlayed) // if the sound has not yet played
	{
		if (OpenSound)
		{
			UGameplayStatics::PlaySound2D(this, OpenSound);
			bSoundPlayed = true;
		}
	}
}

