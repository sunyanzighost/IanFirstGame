// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityBase.h"
#include "GameplayTagsManager.h"
#include "MainCharacter.h"
#include "MainCharacterAttributeSet.h"
#include "Kismet/GameplayStatics.h"

FGameplayAbilityInfo::FGameplayAbilityInfo()
	:CoolDownDuration(0.f),
	CostToCast(0.f),
	AbilityCostType(ECostType::ECT_MAX)
{
}

FGameplayAbilityInfo::FGameplayAbilityInfo(float InCoolDownDuration, float InCostToCast, ECostType InAbilityCostType)
	:CoolDownDuration(InCoolDownDuration),
	CostToCast(InCostToCast),
	AbilityCostType(InAbilityCostType)
{
}

// Default Constructor
UGameplayAbilityBase::UGameplayAbilityBase()
{
	AbilityTags.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability")));
	CancelAbilitiesWithTag.AddTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Ability")));
}

FGameplayAbilityInfo UGameplayAbilityBase::GetAbilityInfo()
{
	// Get hold of the CoolDown & Cost gameplay effect
	UGameplayEffect* CoolDownEffect = GetCooldownGameplayEffect();
	UGameplayEffect* CostEffect = GetCostGameplayEffect();

	if (CoolDownEffect && CostEffect)
	{
		// Get the cooldown duration from the CoolDownEffect
		float Duration;
		CoolDownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1.f, Duration);

		// Get the CostToCast and the AbilityCostType
		float Cost = 0.f;
		ECostType CostType = ECostType::ECT_MAX;

		if(CostEffect->Modifiers.Num() > 0)
		{
			const FGameplayModifierInfo EffectModifierInfo = CostEffect->Modifiers[0];
			EffectModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1.f, Cost);

			const FString AttributeBeginChanged = EffectModifierInfo.Attribute.GetName();

			if(AttributeBeginChanged == "Health")
			{
				CostType = ECostType::ECT_Health;
			}
			else
			{
				CostType = ECostType::ECT_Mana;
			}
		}

		return FGameplayAbilityInfo(Duration, Cost, CostType);
	}
	else
	{
		return FGameplayAbilityInfo();
	}
}

bool UGameplayAbilityBase::HasEnoughAttributeToCast()
{
	FGameplayAbilityInfo AbilityInfo = GetAbilityInfo();

	float CostRequired = AbilityInfo.CostToCast * -1;
	
	// Get hold of the player character ref.
	PlayerCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Check if the player has enough attribute to cast
	if(AbilityInfo.AbilityCostType == ECostType::ECT_Mana)
	{
		if(PlayerCharacter->GetAttributeSetComponent()->Mana.GetCurrentValue() >= CostRequired)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
