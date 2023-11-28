// Fill out your copyright notice in the Description page of Project Settings.


#include "SplicerAnim.h"

#include "SplicerEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

USplicerAnim::USplicerAnim()
{
}

void USplicerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Me = Cast<ASplicerEnemy>(GetOwningActor());
	if (Me)
	{
		Movement = Me->GetCharacterMovement();
	}
}

void USplicerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Speed = Movement->MaxWalkSpeed;
		SplicerHP = Me->SplicerHP;
	}
}
