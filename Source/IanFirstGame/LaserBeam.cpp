// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserBeam.h"


#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ALaserBeam::ALaserBeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkelealMesh"));
	SkeletalMesh->SetupAttachment(Root);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Root);

	LaserEndSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LaserEndSphere"));
	LaserEndSphere->SetupAttachment(SpringArm);

	// Default values
	SpringArm->TargetArmLength = 5000.f;

	LaserEndSphere->SetSphereRadius(64.f);
}

// Called when the game starts or when spawned
void ALaserBeam::BeginPlay()
{
	Super::BeginPlay();

	FVector LaserEndBoneLocation = SkeletalMesh->GetSocketLocation(FName("end"));

	OriginalLaserLength = FVector::Distance(LaserEndBoneLocation, GetActorLocation());
}

// Called every frame
void ALaserBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector LaserEndSphereLocation = LaserEndSphere->GetComponentLocation();

	float CurrentSpringArmLength = FVector::Distance(LaserEndSphereLocation, GetActorLocation());

	LaserBeamOffset = CurrentSpringArmLength - OriginalLaserLength;
}

