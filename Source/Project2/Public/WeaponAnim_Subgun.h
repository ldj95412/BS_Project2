// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponAnim_Subgun.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT2_API UWeaponAnim_Subgun : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* SubgunReloadAnimMontage;

	void PlaySubgunReloadAnim();
};
