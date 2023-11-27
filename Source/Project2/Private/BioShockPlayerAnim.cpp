// Fill out your copyright notice in the Description page of Project Settings.


#include "BioShockPlayerAnim.h"
#include "BioshockPlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>

void UBioShockPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{

	// 오너를 가져와서 유효성 체크를 하고싶다
	if (nullptr == player)
	{
		auto owner = TryGetPawnOwner();
		player = Cast<ABioshockPlayer>(owner);
	}

	if (nullptr == player)
	{	
		return;
	}
	FVector forward = player->GetActorForwardVector();
	FVector right = player->GetActorRightVector();
	FVector velocity = player->GetVelocity();
	Speed = FVector::DotProduct(forward, velocity);
	Direction = FVector::DotProduct(right, velocity);
	UCharacterMovementComponent* movement = player->GetCharacterMovement();
	//isFalling = movement->IsFalling();

	GunState = player->GunState; // 에니메이션 상태 변화, cast한 후에 써야함

}

void UBioShockPlayerAnim::PlayPistolReloadAnim()
{
	Montage_Play(PistolReloadAnimMontage);
}

void UBioShockPlayerAnim::PlayShotgunReloadAnim()
{
	Montage_Play(ShotgunReloadAnimMontage);
}


void UBioShockPlayerAnim::PlayPistolFireAnim()
{
	Montage_Play(PistolFireAnimMontage);
}

void UBioShockPlayerAnim::PlayShotgunFireAnim()
{
	Montage_Play(ShotgunFireAnimMontage);
}

void UBioShockPlayerAnim::PlayElectricFireAnim()
{
	Montage_Play(ElectricFireAnimMontage);
}

//void UBioShockPlayerAnim::NativeInitializeAnimation(float DeltaSeconds)
//{
//	
//}

