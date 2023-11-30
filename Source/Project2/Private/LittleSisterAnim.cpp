// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleSisterAnim.h"

#include "LittleSister.h"
#include "SplicerAnim.h"
#include "GameFramework/CharacterMovementComponent.h"

ULittleSisterAnim::ULittleSisterAnim()
{
}

void ULittleSisterAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Me = Cast<ALittleSister>(GetOwningActor());
	if (Me)
	{
		Movement = Me->GetCharacterMovement();
	}
}

void ULittleSisterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Movement)
	{
		Speed = Movement->MaxWalkSpeed;
	}
}
