// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

UCLASS()
class IANFIRSTGAME_API AFloorSwitch : public AActor
{
	GENERATED_BODY()

private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SwitchMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* SwitchCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Switch", meta = (AllowPrivateAccess = "true"))
	AActor* FenceSwitch;

	// Initial switch location variable
	FVector InitialSwitchLocation;

	// Boolean for knowing whether to lower/ raise the switch
	bool bLowerSwitch;

	bool bRaiseSwitch;

	// How lower be the switch will fall
	float DeltaZ;

	// How fast will the switch lower/ raise
	float SwitchRaiseLowerSpeed;
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// When begin collide with the SwitchCollision
	UFUNCTION()
	void OnSwitchCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// When end collide with the SwitchCollision
	UFUNCTION()
	void OnSwitchCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Lower the switch
	void LowerSwitch(float DeltaTime);

	// Raise the switch
    void RaiseSwitch(float DeltaTime);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
