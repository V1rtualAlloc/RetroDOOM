// Fill out your copyright notice in the Description page of Project Settings.


#include "RetroFPSAttributeSet.h"
#include "RetroFPSCharacter.h"

URetroFPSAttributeSet::URetroFPSAttributeSet()
{

}

void URetroFPSAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	ARetroFPSCharacter* RetroChar = Cast<ARetroFPSCharacter>(GetOwningActor());

	if (Attribute == GetHealthAttribute())
		NewValue = FMath::Clamp<float>(NewValue, 0, RetroChar->MaxHealth);
	else if (Attribute == GetArmorAttribute())
		NewValue = FMath::Clamp<float>(NewValue, 0, RetroChar->MaxArmor);
	else if (Attribute == GetBulletsAttribute())
		NewValue = FMath::Clamp<float>(NewValue, 0, RetroChar->MaxBullets);
	else if (Attribute == GetRocketsAttribute())
		NewValue = FMath::Clamp<float>(NewValue, 0, RetroChar->MaxRockets);
}

bool URetroFPSAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	float AbsoluteMagnitude = FMath::Abs(Data.EvaluatedData.Magnitude);

	ARetroFPSCharacter* RetroChar = Cast<ARetroFPSCharacter>(GetOwningActor());

	if (Data.EvaluatedData.Magnitude > 0)
	{
		if (Data.EvaluatedData.Attribute == GetArmorAttribute())
		{
			if (GetArmor() + AbsoluteMagnitude >= RetroChar->MaxArmor)
			{
				SetArmor(RetroChar->MaxArmor);
				return false;
			}
			else
			{
				Armor.SetCurrentValue(Armor.GetCurrentValue() + AbsoluteMagnitude);
				return false;
			}
		}
		else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
		{
			if (GetHealth() + AbsoluteMagnitude >= RetroChar->MaxHealth)
			{
				SetHealth(RetroChar->MaxHealth);
				return false;
			}
			else
			{
				Health.SetCurrentValue(Health.GetCurrentValue() + AbsoluteMagnitude);
				return false;
			}
		}
		else if (Data.EvaluatedData.Attribute == GetBulletsAttribute())
		{
			if (GetBullets() + AbsoluteMagnitude >= RetroChar->MaxBullets)
			{
				SetBullets(RetroChar->MaxBullets);
				return false;
			}
			else
			{
				Bullets.SetCurrentValue(Bullets.GetCurrentValue() + AbsoluteMagnitude);
				return false;
			}
		}
		else if (Data.EvaluatedData.Attribute == GetRocketsAttribute())
		{
			if (GetRockets() + AbsoluteMagnitude >= RetroChar->MaxRockets)
			{
				SetRockets(RetroChar->MaxRockets);
				return false;
			}
			else
			{
				Rockets.SetCurrentValue(Rockets.GetCurrentValue() + AbsoluteMagnitude);
				return false;
			}
		}
	}
	else
	{	
		if (Data.EvaluatedData.Attribute == GetArmorAttribute() && AbsoluteMagnitude > GetArmor())
			Health.SetCurrentValue(Health.GetCurrentValue() - (AbsoluteMagnitude - GetArmor()));
	}

	return true;
}

void URetroFPSAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetHealthAttribute() && GetHealth() < 0)
		SetHealth(0);
	else if (Data.EvaluatedData.Attribute == GetArmorAttribute() && GetArmor() < 0)
		SetArmor(0);
	else if (Data.EvaluatedData.Attribute == GetBulletsAttribute() && GetBullets() < 0)
		SetBullets(0);
	else if (Data.EvaluatedData.Attribute == GetRocketsAttribute() && GetRockets() < 0)
		SetRockets(0);
}