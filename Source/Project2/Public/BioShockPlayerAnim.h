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
	class UAnimMontage* PistolReloadAnimMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* ShotgunReloadAnimMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* PistolFireAnimMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* ShotgunFireAnimMontage;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* ElectricFireAnimMontage;

	void PlayPistolReloadAnim();
	void PlayShotgunReloadAnim();

	void PlayPistolFireAnim();
	void PlayShotgunFireAnim();
	void PlayElectricFireAnim();



};
