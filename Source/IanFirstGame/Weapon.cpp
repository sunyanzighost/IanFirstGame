// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "MainCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Enemy.h"
#include "FenceSwitch.h"
#include "HiddenStair.h"
#include "HiddenStairSteps.h"

// Sets default values for this actor's properties
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Setup components
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	CombatBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatBox"));
	CombatBox->SetupAttachment(SkeletalMesh);

	// Set collision response to pawn only
	CombatBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CombatBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Set up default values
	Damage = 30.f;

	bLowerSwordSwitch = false;

	bAlreadySpawnHiddenStairSteps = false;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bLowerSwordSwitch)
	{
		LowerSwitches(DeltaTime);
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Bind the functions to call when overlap
	CombatBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatBoxBeginOverlap);

	// Add the switches to the array
	if(Switch1 && Switch2 && Switch3)
	{
		SwitchArray.Add(Switch1);
		SwitchArray.Add(Switch2);
		SwitchArray.Add(Switch3);
	}
}

// On begin overlap for the collision volume
void AWeapon::OnCollisionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCollisionVolumeBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(OtherActor))
	{
		PlayerCharacter->SetOverlappingItem(this);
	}
}

// When the sword hits enemy
void AWeapon::CombatBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(GetOwner()); // Get hold of a player character ref.

	if (!PlayerCharacter) return; //Early return if no ref.

	if (PlayerCharacter->IsAttacking()) // Only deal damage when the player is attacking
	{
		if (AEnemy* Enemy = Cast<AEnemy>(OtherActor)) // Only deal damage when the overlapped actor is an enemy
		{
			if (!WeaponInstigator || !DamageType) return; // Early return if no ref.
			UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstigator, this, DamageType);
			UE_LOG(LogTemp, Warning, TEXT("Current health: %f"), Enemy->GetHealth());

			// Play enemy hit sound
			if (EnemyHitSound)
			{
				UGameplayStatics::PlaySound2D(this, EnemyHitSound);
			}
		}
	}

}

// Equip the weapon to the main character
void AWeapon::Equip(class AMainCharacter* Player)
{
	if (!Player) return; // Early return if there is no player

	// Set the instigator
	SetInstigator(Player->GetController());

	// Set the owner to player
	SetOwner(Player);

	// Setup the collision responses to ignore the pawn and the camera
	SkeletalMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	SkeletalMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// Ignore physics
	SkeletalMesh->SetSimulatePhysics(false);

	// Get hold of the player weapon socket on its skeleton
	const USkeletalMeshSocket* WeaponSocket = Player->GetMesh()->GetSocketByName("WeaponSocket");

	// Attach the weapon to the socket
	// SkeletalMesh->AttachTo(Player->GetMesh(), "WeaponSocket");
	if (!WeaponSocket) return; // Early return if null
	WeaponSocket->AttachActor(this, Player->GetMesh());

	// Set/ reset the weapon reference of the player
	Player->SetEquippedWeapon(this);

	// Stop the rotation when equipped
	this->bRotate = false;

	// Deactivate the idle particle of the when equip
	IdleParticle->Deactivate();

	// Play a sound for equip
	UGameplayStatics::PlaySound2D(this, PickupSound);

	// Lower the switches
	bLowerSwordSwitch = true;
}

// Play swing sound
void AWeapon::PlaySwingSound()
{
	if (!SwingSound)return; // Early return if no ref.
	UGameplayStatics::PlaySound2D(this, SwingSound);
}

// Lower the Switches
void AWeapon::LowerSwitches(float DeltaTime)
{
	// Lower the 3 fences
	if(SwitchArray.Num() != 0)
	{
		for(auto Switch : SwitchArray)
		{
			AFenceSwitch* FenceSwitchActor = Cast<AFenceSwitch>(Switch);
			FenceSwitchActor->LowerFence(DeltaTime);
		}
	}

	// Show the hidden stair steps
	if(HiddenStairSteps && !bAlreadySpawnHiddenStairSteps)
	{
		AHiddenStairSteps* HiddenStairStepsActor = GetWorld()->SpawnActor<AHiddenStairSteps>(HiddenStairSteps);
		bAlreadySpawnHiddenStairSteps = true;
	}
}
