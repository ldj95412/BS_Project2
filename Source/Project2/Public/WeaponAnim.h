// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponAnim.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT2_API UWeaponAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* ShotgunReloadAnimMontage;

	void PlayShotgunReloadAnim();

};
