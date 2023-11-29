// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BigDaddyFSM.generated.h"

UENUM(BlueprintType)
enum class EBigDaddyNormalState : uint8
{
	//Normal Mode
	Idle,
	EyesOn,
	Walk,
	Pushing,
};

UENUM(BlueprintType)
enum class EBigDaddyRageState : uint8
{
	//Rage Mode
	Roaring,
	Run,
	Attack,
	Dying,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT2_API UBigDaddyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBigDaddyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* 클래스 포인터 변수 참조 */
	UPROPERTY() //나의 위치를 확인하는데 이용
	TObjectPtr<class ABigDaddyCharacter> BigDaddyMe;

	UPROPERTY() //주로 플레어이를 참조하여 상대의 위치를 확인하는데 이용.
	TObjectPtr<class ABioshockPlayer> PlayerTarget;
	
	/* 열거형 상태머신 구현 */
	UPROPERTY()
	int32 ChangeState = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EBigDaddyNormalState NormalState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EBigDaddyRageState RageState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeleeAttackRange = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MiddleRange = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LongRange = 800.f;

	float DirectionRotate = 0.f;
	float EyeSightRange = 1000.f;

	void TickIdle();
	void TickWalk();
	void TickEyesOn();
	void TickPushing();
	void TickRoaring();
	void TickRun();
	void TickAttack();
	void TickDying();

	float CurrentTime;
	int8 DoOncePushing;
	int8 DoOnceRoaring;
	int8 DoOnceAttack;
	int8 DoOnceDying;	

	/* 애니메이션 몽타주 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> PushingAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> RoaringAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> AttackActionPunch;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> AttackActionJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> AttackActionSweep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> DyingAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> AttackJeet1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> AttackJeet2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> SpearAttack1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> SpearAttack2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> SpearAttack3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> ElectricStun;

	/* Delay, Timer 대용으로 쓰이는 CurrentTime */
	UPROPERTY()
	float FSMCurrentTime = 0;

	/* 카메라셰이킹 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UCameraShakeBase> CameraShaking;

	/* 효과음 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Character)
	USoundBase* RoarSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Character)
	USoundBase* AtkSweepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Character)
	USoundBase* AtkJumpSound;

	/* 루트 모션을 코드로 구현 */
	UPROPERTY()
	FVector AnimStartLocation;

	UPROPERTY()
	FVector AnimEndLocation;

	/* 빅대디 ABP를 가져와 Notify 및 몽타주 실행에 를 이용 */
	UPROPERTY()
	TObjectPtr<UAnimInstance> BigDaddyAnimInstance;

	/* 애니메이션 노티파이 구현부 */
	UFUNCTION()
	void JumpStarted(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	
	UFUNCTION()
	void JumpEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void Jeet1Started(UAnimMontage* Montage);

	UFUNCTION()
	void Jeet1Ended(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void Jeet2Started(UAnimMontage* Montage);

	UFUNCTION()
	void Jeet2Ended(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void Spear1Started(UAnimMontage* Montage);

	UFUNCTION()
	void Spear1Ended(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void Spear2Started(UAnimMontage* Montage);

	UFUNCTION()
	void Spear2Ended(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void Spear3Started(UAnimMontage* Montage);

	UFUNCTION()
	void Spear3Ended(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION()
	void AttackStarted(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void AttackEnded(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
};