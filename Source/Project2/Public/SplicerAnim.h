// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SplicerAnim.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT2_API USplicerAnim : public UAnimInstance
{
	GENERATED_BODY()

	USplicerAnim();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Character)
	class ASplicerEnemy* Me;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Character)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Character)
	float SplicerHP;
};
