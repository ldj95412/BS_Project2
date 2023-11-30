// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LittleSisterAnim.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT2_API ULittleSisterAnim : public UAnimInstance
{
	GENERATED_BODY()
	
	ULittleSisterAnim();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Character)
	class ALittleSister* Me;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Character)
	float Speed;
};
