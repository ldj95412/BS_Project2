// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnim.h"


void UWeaponAnim::PlayShotgunReloadAnim()
{
	if (ShotgunReloadAnimMontage){
		Montage_Play(ShotgunReloadAnimMontage);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("UWeaponAnim::PlayShotgunReloadAnim"));
	}
}

