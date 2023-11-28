// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SplicerEnemy.generated.h"

UCLASS()
class PROJECT2_API ASplicerEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASplicerEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SplicerHP = 10;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* AttackColl;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USplicerFSM* MySplicerFSM;

	UFUNCTION() //총알과 부딪힐 때, HP-- 및 기타 상호작용을 수행함.
	void OnBulletCompBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);
};
