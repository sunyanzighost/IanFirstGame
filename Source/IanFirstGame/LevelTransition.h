// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "LevelTransition.generated.h"

UCLASS()
class IANFIRSTGAME_API ALevelTransition : public AActor
{
	GENERATED_BODY()

private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* TransitionVolume;

	// Level name variable
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Transition", meta = (AllowPrivateAccess = "true"))
	FName LevelName;
	
public:	
	// Sets default values for this actor's properties
	ALevelTransition();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// When begin collide with the TransitionVolume
	UFUNCTION()
    void OnTransitionVolumeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
