// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Enemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainPlayerController.h"
#include "IanFirstGameSaveGame.h"
#include "IanFirstGame_GameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WeaponStorage.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	// Make the spring arm according to the controller rotation
	SpringArm->bUsePawnControlRotation = true;

	// Set up camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	// Make the character DOES NOT move according to the controller yaw
	bUseControllerRotationYaw = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input ...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f); // ... at this rotation rate

	// Default values
	InterpToEnemySpeed = 10.f;

	MaxHealth = 100.f;
	Health = MaxHealth;

	MaxStamina = 200.f;
	Stamina = MaxStamina;

	SprintingSpeed = 950.f;
	RunningSpeed = 600.f;

	StaminaRate = 25.f;

	MinSprintRequriedStamina = 75.f;

	PlayerStatus = EPlayerStatus::EPS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	MaxSoul = 99999;
	Soul = 0;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Hide the default weapon which comes with the skeletal mesh
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	// Get hold of the main player controller
	MainPlayerController = Cast<AMainPlayerController>(GetController());

	// Get hold of the game instance
	GameInstance = Cast<UIanFirstGame_GameInstance>(GetGameInstance());
	
	if(GameInstance)
	{
		if(GameInstance->GetLoadGamePressed())
		{
			LoadGameNoSwitchLevel();
		}

		if(GameInstance->GetLevelSwitched())
		{
			LoadGameAfterSwitchLevel(TEXT("LevelSwitchSave"), 0);
		}

		GameInstance->SetLevelSwitched(false);
	}
}

// Increment health
void AMainCharacter::IncrementHealth(float Amount)
{
	Health += Amount;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
}

// Increment soul
void AMainCharacter::IncrementSoul(int32 Amount)
{
	Soul += Amount;
	Soul = FMath::Clamp(Soul, 0, MaxSoul);
}

// Saving game
void AMainCharacter::SaveGame()
{
	// Create a save game object
	UIanFirstGameSaveGame* SaveGameInstance = Cast<UIanFirstGameSaveGame>(UGameplayStatics::CreateSaveGameObject(UIanFirstGameSaveGame::StaticClass()));

	SavePlayerStatusAndWeapon(SaveGameInstance);

	SavePlayerLocationAndLevelName(SaveGameInstance);

	// Save the object to the slot with a name and userindex
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SlotName, SaveGameInstance->UserIndex);
}

void AMainCharacter::SaveGameBeforeSwitchLevel(FString SlotName, int32 UserIndex)
{
	// Create a save game object
	UIanFirstGameSaveGame* SaveGameInstance = Cast<UIanFirstGameSaveGame>(UGameplayStatics::CreateSaveGameObject(UIanFirstGameSaveGame::StaticClass()));

	SavePlayerStatusAndWeapon(SaveGameInstance);

	// Save the object to the slot with a name and userindex
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, UserIndex);
}

// Saving player status and weapon
void AMainCharacter::SavePlayerStatusAndWeapon(UIanFirstGameSaveGame* SaveGame)
{
	// Store the player stats to the struct in the save game object
	SaveGame->PlayerCharacterStats.MaxHealth = MaxHealth;
	SaveGame->PlayerCharacterStats.Health = Health;
	SaveGame->PlayerCharacterStats.MaxStamina = MaxStamina;
	SaveGame->PlayerCharacterStats.Stamina = Stamina;
	SaveGame->PlayerCharacterStats.MaxSoul = MaxSoul;
	SaveGame->PlayerCharacterStats.Soul = Soul;

	// Save the weapon if the player is equipping one
	if (EquippedWeapon)
	{
		SaveGame->PlayerCharacterStats.WeaponName = EquippedWeapon->GetWeaponName();
	}
}

// Saving player location and level name
void AMainCharacter::SavePlayerLocationAndLevelName(UIanFirstGameSaveGame* SaveGame)
{
	// Save also the player location and rotation
	SaveGame->PlayerCharacterStats.PlayerLocation = GetActorLocation();
	SaveGame->PlayerCharacterStats.PlayerRotation = GetActorRotation();

	// Save also the current mapname
	SaveGame->PlayerCharacterStats.LevelName = GetCurrentMapName();	
}

// Loading game
void AMainCharacter::LoadGame()
{
	GameInstance->SetLoadGamePressed(true);
	
	// Create a load game object
	UIanFirstGameSaveGame* LoadGameInstance = Cast<UIanFirstGameSaveGame>(UGameplayStatics::CreateSaveGameObject(UIanFirstGameSaveGame::StaticClass()));

	// Load the data into the game object just created
	LoadGameInstance = Cast<UIanFirstGameSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SlotName, LoadGameInstance->UserIndex));

	LoadPlayerStatusAndWeapon(LoadGameInstance);

	UpdateCharacterControlAfterLoadGame();

	// Set the player location and rotation, if the player is within the same map
	if (GetCurrentMapName() == LoadGameInstance->PlayerCharacterStats.LevelName) // if the player is in the same map
	{
		SetActorLocation(LoadGameInstance->PlayerCharacterStats.PlayerLocation);
		SetActorRotation(LoadGameInstance->PlayerCharacterStats.PlayerRotation);
	}
	else // Switch level
	{
		SwitchLevel(FName(LoadGameInstance->PlayerCharacterStats.LevelName));
	}
}

// Loading game without switching level
void AMainCharacter::LoadGameNoSwitchLevel()
{	
	// Create a load game object
	UIanFirstGameSaveGame* LoadGameInstance = Cast<UIanFirstGameSaveGame>(UGameplayStatics::CreateSaveGameObject(UIanFirstGameSaveGame::StaticClass()));

	// Load the data into the game object just created
	LoadGameInstance = Cast<UIanFirstGameSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SlotName, LoadGameInstance->UserIndex));

	LoadPlayerStatusAndWeapon(LoadGameInstance);

	UpdateCharacterControlAfterLoadGame();

	// Set the player location and rotation, if the player is within the same map
	if (GetCurrentMapName() == LoadGameInstance->PlayerCharacterStats.LevelName) // if the player is in the same map
	{
		SetActorLocation(LoadGameInstance->PlayerCharacterStats.PlayerLocation);
		SetActorRotation(LoadGameInstance->PlayerCharacterStats.PlayerRotation);
	}
}

void AMainCharacter::LoadGameAfterSwitchLevel(FString SlotName, int32 UserIndex)
{
	// Create a load game object
	UIanFirstGameSaveGame* LoadGameInstance = Cast<UIanFirstGameSaveGame>(UGameplayStatics::CreateSaveGameObject(UIanFirstGameSaveGame::StaticClass()));

	// Load the data into the game object just created
	LoadGameInstance = Cast<UIanFirstGameSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));

	LoadPlayerStatusAndWeapon(LoadGameInstance);

	UpdateCharacterControlAfterLoadGame();
}

void AMainCharacter::LoadPlayerStatusAndWeapon(UIanFirstGameSaveGame* LoadGame)
{
	// Set the player current stats with the data in the load game object
	MaxHealth = LoadGame->PlayerCharacterStats.MaxHealth;
	Health = LoadGame->PlayerCharacterStats.Health;
	MaxStamina = LoadGame->PlayerCharacterStats.MaxStamina;
	Stamina = LoadGame->PlayerCharacterStats.Stamina;
	MaxSoul = LoadGame->PlayerCharacterStats.MaxSoul;
	Soul = LoadGame->PlayerCharacterStats.Soul;

	// Set the player weapon if there is a weapon saved and a valid ref. to weapon storage
	if (WeaponStorage)
	{
		AWeaponStorage* AllWeapons = GetWorld()->SpawnActor<AWeaponStorage>(WeaponStorage);

		FString WeaponName = LoadGame->PlayerCharacterStats.WeaponName;

		if (AllWeapons->WeaponStorageMap.Contains(WeaponName)) // Check if the weapon storage does contain the saved weapon name
		{
			AWeapon* LoadedWeapon = GetWorld()->SpawnActor<AWeapon>(AllWeapons->WeaponStorageMap[WeaponName]); // Use the weapon name as the key to locate the matching weapon blueprint

			LoadedWeapon->Equip(this);
		}
	}
}

void AMainCharacter::UpdateCharacterControlAfterLoadGame()
{
	// Take care load game if the player already dead
	SetPlayerStatus(EPlayerStatus::EPS_Normal);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	EnableInput(PlayerController);

	// Get rid of the pause menu
	if (MainPlayerController)
	{
		MainPlayerController->HidePauseMenu();
	}
}

// Quitting game
void AMainCharacter::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, MainPlayerController, EQuitPreference::Quit, true);
}

// Switching level
void AMainCharacter::SwitchLevel(FName LevelName)
{
	FName CurrentLevelName = FName(GetCurrentMapName());	
	if(CurrentLevelName == LevelName) return; // Early return if the player is already at the level
	
	GameInstance->SetLevelSwitched(true);

	SaveGameBeforeSwitchLevel(TEXT("LevelSwitchSave"), 0);	

	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

FString AMainCharacter::GetCurrentMapName()
{
	FString MapName = GetWorld()->GetMapName();
	
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	return MapName;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaStamina = StaminaRate * DeltaTime;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bSprintKeyDown) // When sprinting
		{
			Stamina -= DeltaStamina;
			Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
			if (Stamina <= MinSprintRequriedStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			}
		}
		else // When not sprinting
		{
			Stamina += DeltaStamina;
			Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bSprintKeyDown) // When sprinting
		{
			Stamina -= DeltaStamina;
			Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
			if (Stamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				SetPlayerStatus(EPlayerStatus::EPS_Normal); // Change the character speed back to normal by setting the PlayerStatus variable
			}
		}
		else // When not sprinting
		{
			Stamina += DeltaStamina;
			Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
			if (Stamina >= MinSprintRequriedStamina) { SetStaminaStatus(EStaminaStatus::ESS_Normal); }
		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bSprintKeyDown)
		{
			Stamina = 0.f;
		}
		else
		{
			SetStaminaStatus(EStaminaStatus::ESS_Recovery);
		}
		break;
	case EStaminaStatus::ESS_Recovery:
		Stamina += DeltaStamina;
		if (Stamina >= MinSprintRequriedStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
		}
		break;
	default:
		;
	}

	// Interping towards the enemy target yaw rotation
	if (bAttacking && EnemyTarget) // Only interp when attacking and there is a valid enemy target
	{
		FRotator TargetRotation = GetLookAtEnemyTargetYawRotation(EnemyTarget);
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, InterpToEnemySpeed);
		SetActorRotation(NewRotation);
	}

	// Set the enemy location for displaying the enemy health bar when there is an enemy target
	if (MainPlayerController && EnemyTarget)
	{
		MainPlayerController->SetEnemyLocation(EnemyTarget->GetActorLocation());
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Moving around
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	// Looking around
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AMainCharacter::LookRight);

	// Jumping
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::Jump);

	// Sprinting
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::StopSprint);

	// Use item
	PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AMainCharacter::UseItem);

	// Pause game
	FInputActionBinding &Toggle  = PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AMainCharacter::PauseGame);
	Toggle.bExecuteWhenPaused = true;

	// Attack
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacter::Attack);
}

// Moving forward/ backward
void AMainCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

// Moving right/ left
void AMainCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

// Looking up/ down
void AMainCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

// Looking right/ left
void AMainCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

// Jump
void AMainCharacter::Jump()
{
	Super::Jump();
}

// Sprinting
void AMainCharacter::Sprint()
{
	bSprintKeyDown = true;

	if (Stamina < MinSprintRequriedStamina) return; // Early return if the stamina has not reach the min. requirement for sprinting
	SetPlayerStatus(EPlayerStatus::EPS_Sprinting); // Change the character speed to sprinting speed
}

void AMainCharacter::StopSprint()
{
	bSprintKeyDown = false;
	SetPlayerStatus(EPlayerStatus::EPS_Normal); // Change the character speed back to normal
}

// Use item
void AMainCharacter::UseItem()
{
	if (AWeapon* Weapon = Cast<AWeapon>(OverlappingItem))
	{
		Weapon->Equip(this);
		SetOverlappingItem(nullptr);
	}
}

void AMainCharacter::PauseGame()
{
	if (MainPlayerController)
	{
		MainPlayerController->TogglePauseMenu();
	}
}

// Set the closest enemy to be the enemy target (and shown the health bar of this particular target) when there are multiple enemies around the player
void AMainCharacter::SetClosestEnemyAsTarget()
{
	TArray<AActor*> OverlappingEnemies;
	GetOverlappingActors(OverlappingEnemies, EnemyFilter); // Get hold of all the overlapping enemies and store in a Tarray

	if (OverlappingEnemies.Num() == 0) // Early return if there is no overlapping enemy
	{
		if (MainPlayerController)
		{
			MainPlayerController->HideEnemyHealthBar(); // Hide the enemy health bar
			MainPlayerController->SetEnemyTargetForHealthBar(nullptr); // Clear the enemy target for the health bar
		}
		return;
	}

	AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingEnemies[0]); // Assume the first enemy in the array as the closest one first

	FVector PlayerLocation = GetActorLocation();  // Get the location of the player

	if (!ClosestEnemy) return; // Early return if there is no ref.

	float ClosestDistance = FVector::Distance(PlayerLocation, ClosestEnemy->GetActorLocation()); // Calculate the distance between the player and the "closest enemy"

	for (auto Actor : OverlappingEnemies) // Loop through the whole array to see if there is any distance closer than the above calculated one
	{
		AEnemy* Enemy = Cast<AEnemy>(Actor);

		float Distance = FVector::Distance(PlayerLocation, Enemy->GetActorLocation());

		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestEnemy = Enemy;
		}
	}
	EnemyTarget = ClosestEnemy; // Set the closest one to be the current target

	if (MainPlayerController)
	{
		MainPlayerController->ShowEnemyHealthBar();
		MainPlayerController->SetEnemyTargetForHealthBar(ClosestEnemy);
	}
}

// Get the yaw rotation in the direction of the player to the enemy target
FRotator AMainCharacter::GetLookAtEnemyTargetYawRotation(AEnemy* Target)
{
	FRotator LookAtEnemyTargetYawRotation= UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	return FRotator(0.f, LookAtEnemyTargetYawRotation.Yaw, 0.f);
}

// Attack
void AMainCharacter::Attack()
{
	if (EquippedWeapon && !bAttacking) // Only attack when there is a valid weapon ref. && not in the action of attacking
	{
		bAttacking = true;

		// Get the animinstance of the player
		UAnimInstance* PlayerAnimInstance = GetMesh()->GetAnimInstance();

		if (!PlayerAnimInstance || !CombatMontage) return; // Early return if no ref.

		if (!CombatMontage) return; // Early return if no ref.

		int32 Skill = FMath::RandRange(0, CombatMontage->CompositeSections.Num() - 1);

		PlayerAnimInstance->Montage_Play(CombatMontage, 1.0f);

		FName SectionName = CombatMontage->CompositeSections[Skill].SectionName;

		PlayerAnimInstance->Montage_JumpToSection(SectionName, CombatMontage);

		// Play swing sound
		EquippedWeapon->PlaySwingSound();
	}
}

// When end attacking
void AMainCharacter::AttackEnd()
{
	bAttacking = false;
}

// Taking damage
float AMainCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (PainSound)
	{
		UGameplayStatics::PlaySound2D(this, PainSound);
	}
	Health -= Damage;
	Health = FMath::Clamp(Health, 0.f, MaxHealth);

	if (Health <= 0.f)
	{
		Die();
	}

	return Damage;
}

// Die
void AMainCharacter::Die()
{
	SetPlayerStatus(EPlayerStatus::EPS_Dead);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	DisableInput(PlayerController);

	// Play death sound and particles
	if (DeathSound)
	{
		UGameplayStatics::PlaySound2D(this, DeathSound);
	}

	if (DeathParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticle, GetActorLocation(), FRotator(0.f), true);
	}
}

/** Called when the actor falls out of the world 'safely' (below KillZ and such) */
void AMainCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	Die();
}

// Setters for the equipped weapon
void AMainCharacter::SetEquippedWeapon(AWeapon* Weapon)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy(); // Destroy the current weapon if the character is holding one
	}
	
	EquippedWeapon = Weapon;
}

// Set the player status and also the walk speed
void AMainCharacter::SetPlayerStatus(EPlayerStatus Status)
{
	PlayerStatus = Status;

	if (PlayerStatus == EPlayerStatus::EPS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}
