// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BigDaddyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT2_API UBigDaddyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UBigDaddyAnimInstance();
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Character)
	TObjectPtr<class ABigDaddyCharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Character)
	FVector GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Character)
	float MaxSpeed;
};