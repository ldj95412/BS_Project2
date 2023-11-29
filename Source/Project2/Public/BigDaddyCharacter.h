// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BigDaddyCharacter.generated.h"

UCLASS()
class PROJECT2_API ABigDaddyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABigDaddyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* 컴포넌트 생성 */
	UPROPERTY(EditAnywhere) //빅대디 체력 상태에 따라 빛의 색상이 신호등 형태로 바뀜
	TObjectPtr<class UPointLightComponent> HPPntLight;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class URectLightComponent> BigDaddySpotLight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) //빅대디의 AI 컴포넌트
	TObjectPtr<class UBigDaddyFSM> MyFSMComponent;
	
	UPROPERTY(EditAnywhere) //빅대디의 공격 판정 콜리전 컴포넌트
	TObjectPtr<class UCapsuleComponent> AttackCollision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> DrillHand;

	/* 오버랩 이벤트 생성 */
	UFUNCTION() //총알과 부딪힐 때, HP-- 및 기타 상호작용을 수행함.
	void OnBulletCompBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);
	
	UFUNCTION() //플레이어에게 공격 할 때, PlayerHP-- 및 기타 상호작용을 수행함.
	void OnAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);

	/* 클래스 포인터 변수 선언 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Character)//사운드
	USoundBase* AtkPunchSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Character)//사운드
	USoundBase* PlayerHitSound1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Character)//사운드
	USoundBase* PlayerHitSound2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Character)//사운드
	USoundBase* PlayerHitSound3;

	/* 멤버 변수 선언 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) //BigDaddy의 체력
	float HP = 30;
};