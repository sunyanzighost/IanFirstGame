// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserBeam.h"



#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"

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

	DurationBetweenEachDamage = 0.3f;
}

// Called when the game starts or when spawned
void ALaserBeam::BeginPlay()
{
	Super::BeginPlay();

	FVector LaserEndBoneLocation = SkeletalMesh->GetSocketLocation(FName("end"));

	OriginalLaserLength = FVector::Distance(LaserEndBoneLocation, GetActorLocation());

	LaserEndSphere->OnComponentBeginOverlap.AddDynamic(this, &ALaserBeam::LaserEndSphereStartOverlap);
	LaserEndSphere->OnComponentEndOverlap.AddDynamic(this, &ALaserBeam::LaserEndSphereEndOverlap);

	GetWorldTimerManager().SetTimer(LaserDamageTimerHandle, this, &ALaserBeam::SendDamageEvent, DurationBetweenEachDamage, true);
}

// Called every frame
void ALaserBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector LaserEndSphereLocation = LaserEndSphere->GetComponentLocation();

	float CurrentSpringArmLength = FVector::Distance(LaserEndSphereLocation, GetActorLocation());

	LaserBeamOffset = CurrentSpringArmLength - OriginalLaserLength;
}

// On LaserEndSphereStartOverlap with enemies
void ALaserBeam::LaserEndSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AEnemy* Enemy = Cast<AEnemy>(OtherActor))
	{
		OverlappedActors.Add(Enemy);
	}
}

// On LaserEndSphereEndOverlap with enemies
void ALaserBeam::LaserEndSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(AEnemy* Enemy = Cast<AEnemy>(OtherActor))
	{
		OverlappedActors.Remove(Enemy);
	}
}

// Send a damage event to the owner's ability
void ALaserBeam::SendDamageEvent()
{
	// Get hold of the event tag
	FGameplayTag DamageEventTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability.Laser.Damage"));

	// Turn the array into a target data for the ability system
	FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(OverlappedActors, false);

	// Configure the gameplay event data for sending the gameplay event to the owner's ability
	FGameplayEventData DamageEventData;
	DamageEventData.TargetData = TargetDataHandle;
	DamageEventData.Instigator = GetOwner();
	DamageEventData.EventTag = DamageEventTag;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), DamageEventTag, DamageEventData);
}
