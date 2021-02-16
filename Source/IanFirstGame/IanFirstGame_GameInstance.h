// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IanFirstGame_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class IANFIRSTGAME_API UIanFirstGame_GameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	bool bLoadGamePressed;
	bool bLevelSwitched;

public:
	UIanFirstGame_GameInstance();

	FORCEINLINE bool GetLoadGamePressed(){ return bLoadGamePressed; }

	FORCEINLINE void SetLoadGamePressed(bool ButtonPressed) { bLoadGamePressed = ButtonPressed; }

	FORCEINLINE bool GetLevelSwitched(){ return bLevelSwitched; }

	FORCEINLINE void SetLevelSwitched(bool LevelSwitched) { bLevelSwitched = LevelSwitched; }
	
};
