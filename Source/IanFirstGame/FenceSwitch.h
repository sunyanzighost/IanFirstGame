// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"

#include "FenceSwitch.generated.h"

UCLASS()
class IANFIRSTGAME_API AFenceSwitch : public AActor
{
	GENERATED_BODY()

private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	FVector InitialLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fence Switch", meta = (AllowPrivateAccess = "true"))
	float DeltaZ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fence Switch", meta = (AllowPrivateAccess = "true"))
	float InterpSpeed;

	// Boolean for whether the sound is played or not
	bool bSoundPlayed;

	// Sounds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	USoundCue* OpenSound;
	
	
public:	
	// Sets default values for this actor's properties
	AFenceSwitch();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Lower the fence
	void LowerFence(float DeltaTime);

	// Set the bSoundPlayed
	FORCEINLINE void SetSoundPlayed(bool SoundPlayed){bSoundPlayed = SoundPlayed;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
