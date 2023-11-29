// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GunSplicerEnemy.generated.h"


UCLASS()
class PROJECT2_API AGunSplicerEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGunSplicerEnemy();

protected:
	UFUNCTION()
	void OnBulletCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                              const FHitResult& SweepResult);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* GunMesh;

	UPROPERTY(EditDefaultsOnly)
	class UGunSplicerFSM*  GunSplicerFSM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GunSplicerHP = 5;
};
