// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazard.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values for this actor's properties
AHazard::AHazard()
{

}

// On begin overlap for the collision volume
void AHazard::OnCollisionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCollisionVolumeBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(OtherActor))
	{
		ApplyEffectUponPickUp(PlayerCharacter);

		if (PickupParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, PickupParticle, GetActorLocation(), FRotator(0.f));
		}

		if (PickupSound)
		{
			UGameplayStatics::PlaySound2D(this, PickupSound);
		}

		Destroy();
	}

}

// Called when the game starts or when spawned
void AHazard::BeginPlay()
{
	Super::BeginPlay();
}