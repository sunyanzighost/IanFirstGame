// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "HealthRegenerationGameplayCue.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API UHealthRegenerationGameplayCue : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Particles", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* HealthRegenerationParticle;

public:	
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
