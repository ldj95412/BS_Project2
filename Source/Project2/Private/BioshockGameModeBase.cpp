// Fill out your copyright notice in the Description page of Project Settings.


#include "BioshockGameModeBase.h"



void ABioshockGameModeBase::BulletDecrease(int32 point)
{
	CurrentBullet -= point;
	if (CurrentBullet < 0)
	{
		CurrentBullet = 0;
		if (UsableBullet > 0)
		{
			CurrentBullet = LoadingBullet;
		}
		else
		{
			// 총알 발사 불가
		}
	}

}

//
//void ABioshockGameModeBase::NoBullet(int32 point)
//{
//	if (UsableBullet < 0)
//	{
//		UsableBullet = 0;
//		// 총알 발사 불가
//
//	}
//}
