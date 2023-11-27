// Fill out your copyright notice in the Description page of Project Settings.


#include "BigDaddyAnimInstance.h"
#include "BigDaddyCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBigDaddyAnimInstance::UBigDaddyAnimInstance()
{
}

void UBigDaddyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ABigDaddyCharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UBigDaddyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		MaxSpeed = Movement->MaxWalkSpeed;
	}
}