// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BioshockGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT2_API ABioshockGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void BulletDecrease(int32 point);
	//void NoBullet(int32 point);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentBullet = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UsableBullet = 40;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LoadingBullet = 10; 
};
