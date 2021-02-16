// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransition.h"
#include "Components/BoxComponent.h"
#include "MainCharacter.h"

// Sets default values
ALevelTransition::ALevelTransition()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransiitionVolume"));
	RootComponent = TransitionVolume;

	LevelName = "ElvenRuins";

}

// Called when the game starts or when spawned
void ALevelTransition::BeginPlay()
{
	Super::BeginPlay();

	TransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransition::OnTransitionVolumeOverlap);
	
}

// Called every frame
void ALevelTransition::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// When begin collide with the TransitionVolume
void ALevelTransition::OnTransitionVolumeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(OtherActor))
	{
		PlayerCharacter->SwitchLevel(LevelName);
	}
}

