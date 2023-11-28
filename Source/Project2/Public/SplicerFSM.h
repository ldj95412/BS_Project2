// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SplicerFSM.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT2_API USplicerFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USplicerFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	class ASplicerEnemy* Me;

	UPROPERTY()
	class UAnimInstance* SplicerAnim;
	
	UPROPERTY()
	class ABioshockPlayer* MyPlayer;
	
	UPROPERTY()
	int32 SplicerState = 0;
	//0은 경계
	//1은 발견 후 이동
	//2는 빠르게 이동
	//3은 공격
	//.. 반복

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectingRange = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 100;

	void TickIdle();
	void TickWalk();
	void TickRun();
	void TickAttack();
	void TickDying();
	void TickCrawl();
	void TickCrawlAttack();
	void TickTrueDie();
	
	float CurrentTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> DyingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> TrueDyingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> CrawlMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> CrawlAttackMontage;

	UFUNCTION()
	void DyingStarted(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void TrueDyingStarted(UAnimMontage* Montage, bool bInterrupted);

};

