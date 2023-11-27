// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BioshockPlayer.generated.h"

UCLASS()
class PROJECT2_API ABioshockPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABioshockPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	class UPlayerFSM* fsm;

public:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* playerCamComp;

	void Turn(float value);
	void LookUp(float value);

	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 600;
	FVector direction;

	void InputHorizontal(float value);
	void InputVertical(float value);
	void InputJump();
	void Move();
	void ActionFire();
	void ActionReload();
	
	void PrimaryWeapon();
	void SecondaryWeapon();
	void ActionSuperPower();

	void FirePistol();
	void FireShotgun();
	void FireSuperpower();

	UPROPERTY(VisibleAnywhere, Category=GunMesh)
	class USkeletalMeshComponent* PistolComp;

	UPROPERTY(VisibleAnywhere, Category=GunMesh)
	class USkeletalMeshComponent* ShotgunComp;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* SuperpowerEffect;


	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> BulletFactory;
	 
	UPROPERTY(EditAnywhere, Category=BulletEffect)
	class UParticleSystem* bulletEffectFactory;


	UPROPERTY(EditAnywhere)
	class UParticleSystem* lightningVFX;
	FTransform PowerPosition;

	UPROPERTY()
	class UPlayerFSM* MyPlayerFSM;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GunState = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HP = 10;
};
