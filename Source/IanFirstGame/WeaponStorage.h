// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponStorage.generated.h"

UCLASS()
class IANFIRSTGAME_API AWeaponStorage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponStorage();

	// A map for storing different weapon
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TMap<FString, TSubclassOf<class AWeapon>> WeaponStorageMap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
