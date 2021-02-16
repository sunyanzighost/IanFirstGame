// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include "Components/BoxComponent.h"
#include "MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FenceSwitch.h"

// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
	RootComponent = SwitchMesh;

	SwitchCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SwitchCollision"));
	SwitchCollision->SetupAttachment(SwitchMesh);

	bLowerSwitch = false;

	bRaiseSwitch = false;

	DeltaZ = 18.f;

	SwitchRaiseLowerSpeed = 0.5f;
}

// Called when the game starts or when spawned

void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();

	InitialSwitchLocation = SwitchMesh->GetComponentLocation();
	
	SwitchCollision->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnSwitchCollisionOverlap);
	SwitchCollision->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnSwitchCollisionEndOverlap);
}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bLowerSwitch)
	{
		LowerSwitch(DeltaTime);
	}

	if(bRaiseSwitch)
	{
		RaiseSwitch(DeltaTime);
	}

}

// When begin collide with the SwitchCollision
void AFloorSwitch::OnSwitchCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(OtherActor))
	{
		bLowerSwitch = true;

		bRaiseSwitch = false;
		
	}
}

// When end collide with the SwitchCollision
void AFloorSwitch::OnSwitchCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(OtherActor))
	{
		bLowerSwitch = false;

		bRaiseSwitch = true;

		if(FenceSwitch)
		{
			AFenceSwitch* FenceSwitchActor = Cast<AFenceSwitch>(FenceSwitch);
			FenceSwitchActor->SetSoundPlayed(false);
		}
	}
}

// Lower the switch
void AFloorSwitch::LowerSwitch(float DeltaTime)
{
	float CurrentZ = GetActorLocation().Z;
	float TargetZ = InitialSwitchLocation.Z - DeltaZ;
	FVector TargetEndPoint = InitialSwitchLocation;
		
	CurrentZ = FMath::FInterpTo(CurrentZ, TargetZ, DeltaTime, SwitchRaiseLowerSpeed);

	TargetEndPoint.Z = CurrentZ;
	SetActorLocation(TargetEndPoint);

	if(FenceSwitch)
	{
		AFenceSwitch* FenceSwitchActor = Cast<AFenceSwitch>(FenceSwitch);
		FenceSwitchActor->LowerFence(DeltaTime);
	}
}

// Raise the switch
void AFloorSwitch::RaiseSwitch(float DeltaTime)
{
	float CurrentZ = GetActorLocation().Z;
	float TargetZ = InitialSwitchLocation.Z;
	FVector TargetEndPoint = InitialSwitchLocation;
		
	CurrentZ = FMath::FInterpTo(CurrentZ, TargetZ, DeltaTime, SwitchRaiseLowerSpeed);

	TargetEndPoint.Z = CurrentZ;
	
	SetActorLocation(TargetEndPoint);
}