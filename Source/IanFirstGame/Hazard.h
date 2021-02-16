// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Hazard.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API AHazard : public AItem
{
	GENERATED_BODY()

private:
	// Damage Class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hazard", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageClass;

public:
	// Sets default values for this actor's properties
	AHazard();

	// On begin overlap for the collision volume
	virtual void OnCollisionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// Effect upon pickup
	UFUNCTION(BlueprintImplementableEvent, Category = "Hazard")
	void ApplyEffectUponPickUp(class AMainCharacter* Player);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
