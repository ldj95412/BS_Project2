// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerFSM.h"

#include "BioShockPlayerAnim.generated.h"

UCLASS()
class PROJECT2_API UBioShockPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	//EPlayerState animState;

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds);
	//virtual void NativeInitializeAnimation(float DeltaSeconds);


	UPROPERTY()
	class ABioshockPlayer* player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//bool isFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GunState;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* ShotgunArmReloadAnimMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* SubgunArmReloadAnimMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* ShotgunFireAnimMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* KnifeWieldAnimMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* SubgunFireAnimMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* ShotgunEquipAnimMontage;

	void PlayShotgunArmReloadAnim();
	void PlaySubgunArmReloadAnim();

	void PlayShotgunFireAnim();
	void PlayKnifeWieldAnim();
	void PlaySubgunFireAnim(float PlayRate = 1);

	void PlayShotgunEquipAnim();



};
