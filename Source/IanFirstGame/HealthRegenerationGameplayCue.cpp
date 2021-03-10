// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthRegenerationGameplayCue.h"

#include "Kismet/GameplayStatics.h"

bool UHealthRegenerationGameplayCue::OnExecute_Implementation(AActor* MyTarget,
                                                              const FGameplayCueParameters& Parameters) const
{
	Super::OnExecute_Implementation(MyTarget, Parameters);

	if(HealthRegenerationParticle)
	{
		UGameplayStatics::SpawnEmitterAttached(HealthRegenerationParticle, MyTarget->GetRootComponent(), FName("None"), FVector(0.f, 0.f, -40.f), FRotator(0.f), EAttachLocation::SnapToTarget);
	}

	return true;
}
