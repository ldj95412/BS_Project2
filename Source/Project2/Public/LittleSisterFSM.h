// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LittleSisterFSM.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT2_API ULittleSisterFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULittleSisterFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	class ALittleSister* Me;

	UPROPERTY()
	class ABioshockPlayer* Target;

	UPROPERTY()
	TSubclassOf<class ABigDaddyCharacter> BigDaddyFactory;

	UPROPERTY()
	class ABigDaddyCharacter* BigDaddy;
	
	UPROPERTY()
	int32 LittleSisterState = 0;
	// 0은 대기
	// 1은 도망
	// 2는 무방비 상태

	void TickIdle();
	void TickEscape();
	void TickGiveUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectingRange = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EscapeRange = 500;
};
