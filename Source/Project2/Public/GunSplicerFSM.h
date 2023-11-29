// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunSplicerFSM.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT2_API UGunSplicerFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunSplicerFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY()
	class AGunSplicerEnemy* Me;

	UPROPERTY()
	class UAnimInstance* GunSplicerAnim;
	
	UPROPERTY()
	class ABioshockPlayer* MyPlayer;
	
	UPROPERTY()
	int32 GunSplicerState = 0;
	//0은 경계
	//1은 발견 후 이동
	//2은 공격
	//.. 반복

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectingRange = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 1000;

	void TickIdle();
	void TickWalk();
	void TickAttack();
	void TickDying();
	
	float CurrentTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> DyingMontage;
	
	UFUNCTION()
	void DyingStarted(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Character)//사운드
	USoundBase* PlayerHitSound1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Character)//사운드
	USoundBase* PlayerHitSound2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Character)//사운드
	USoundBase* PlayerHitSound3;
};
