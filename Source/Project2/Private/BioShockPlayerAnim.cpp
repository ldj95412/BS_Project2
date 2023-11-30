// Fill out your copyright notice in the Description page of Project Settings.


#include "BioShockPlayerAnim.h"
#include "BioshockPlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>

void UBioShockPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{

	// ���ʸ� �����ͼ� ��ȿ�� üũ�� �ϰ�ʹ�
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

	GunState = player->GunState; // ���ϸ��̼� ���� ��ȭ, cast�� �Ŀ� �����

}

void UBioShockPlayerAnim::PlayShotgunArmReloadAnim()
{
	if (ShotgunArmReloadAnimMontage) {
		Montage_Play(ShotgunArmReloadAnimMontage);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("UBioShockPlayerAnim::PlayShotgunArmReloadAnim"));
	}
}


void UBioShockPlayerAnim::PlaySubgunArmReloadAnim()
{
	Montage_Play(SubgunArmReloadAnimMontage);
}


void UBioShockPlayerAnim::PlayShotgunFireAnim()
{
	Montage_Play(ShotgunFireAnimMontage);
}

void UBioShockPlayerAnim::PlayKnifeWieldAnim()
{
	Montage_Play(KnifeWieldAnimMontage);
}

void UBioShockPlayerAnim::PlaySubgunFireAnim(float PlayRate)
{
	Montage_Play(SubgunFireAnimMontage);
}

void UBioShockPlayerAnim::PlayShotgunEquipAnim()
{
	Montage_Play(ShotgunEquipAnimMontage);
}

//void UBioShockPlayerAnim::NativeInitializeAnimation(float DeltaSeconds)
//{
//	
//}

