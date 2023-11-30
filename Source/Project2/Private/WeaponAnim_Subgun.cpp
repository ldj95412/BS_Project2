// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnim_Subgun.h"

void UWeaponAnim_Subgun::PlaySubgunReloadAnim()
{
	if (SubgunReloadAnimMontage) {
		Montage_Play(SubgunReloadAnimMontage);
	}
}
