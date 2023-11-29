// Fill out your copyright notice in the Description page of Project Settings.


#include "GunSplicerAnim.h"

#include "GunSplicerEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

UGunSplicerAnim::UGunSplicerAnim()
{
}

void UGunSplicerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Me = Cast<AGunSplicerEnemy>(GetOwningActor());
	if (Me)
	{
		Movement = Me->GetCharacterMovement();
	}
}

void UGunSplicerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Movement)
	{
		Speed = Movement->MaxWalkSpeed;
		SplicerHP = Me->GunSplicerHP;
	}
}
