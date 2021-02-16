// Fill out your copyright notice in the Description page of Project Settings.


#include "HiddenStair.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AHiddenStair::AHiddenStair()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	bRaiseStair = false;

	DeltaZ = 500.f;

	InterpSpeed = 0.5f;
}

// Called when the game starts or when spawned
void AHiddenStair::BeginPlay()
{
	Super::BeginPlay();

	StartPoint = GetActorLocation();
}

// Called every frame
void AHiddenStair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bRaiseStair)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector EndInterpPoint = StartPoint;

		EndInterpPoint.Z += DeltaZ;
		CurrentLocation = FMath::VInterpTo(CurrentLocation, EndInterpPoint, DeltaTime, InterpSpeed);
		SetActorLocation(CurrentLocation);

		if(FVector::Distance(CurrentLocation, EndInterpPoint) <= 5.f)
		{
			bRaiseStair = false;
		}
	}
}

void AHiddenStair::RaiseStair()
{
	bRaiseStair = true;

	if(RockRaisingSound)
	{
		UGameplayStatics::PlaySound2D(this, RockRaisingSound);
	}
}

