// Fill out your copyright notice in the Description page of Project Settings.


#include "GunSplicerFSM.h"

#include "BioshockPlayer.h"
#include "GunSplicerEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UGunSplicerFSM::UGunSplicerFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGunSplicerFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	MyPlayer = Cast<ABioshockPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	Me = Cast<AGunSplicerEnemy>(GetOwner());
	GunSplicerAnim = Me->GetMesh()->GetAnimInstance();

	GunSplicerAnim->OnPlayMontageNotifyBegin.AddDynamic(this, &UGunSplicerFSM::DyingStarted);
}


// Called every frame
void UGunSplicerFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (GunSplicerState == 0) //정지 - 경계 모드
	{
		TickIdle();
	}
	else if (GunSplicerState == 1) //걷기 모드
	{
		TickWalk();
	}
	else if (GunSplicerState == 2) //공격 모드
	{
		TickAttack();
	}
	else if (GunSplicerState == 3) //사망 모드
	{
		TickDying();
	}
}

void UGunSplicerFSM::TickIdle()
{
	FVector MyDir = MyPlayer->GetActorLocation() - Me->GetActorLocation();
	if (MyPlayer && MyDir.Size() < DetectingRange)
	{
		GunSplicerState = 1; 
	}
}

void UGunSplicerFSM::TickWalk()
{
	Me->GetCharacterMovement()->MaxWalkSpeed = 150;
	FVector MyDir = MyPlayer->GetActorLocation() - Me->GetActorLocation();
	if (Me->GunSplicerHP < 1)
	{
		GunSplicerState = 3;
	}
	Me->AddMovementInput(MyDir.GetSafeNormal());
	if (MyDir.Size() < AttackRange)
	{
		GunSplicerState = 2;
	}
	else
	{
		GunSplicerState = 1;
	}
}

void UGunSplicerFSM::TickAttack()
{
	Me->GetCharacterMovement()->MaxWalkSpeed = 0;
	FVector MyDir = MyPlayer->GetActorLocation() - Me->GetActorLocation();
	if (Me->GunSplicerHP < 1)
	{
		GunSplicerState = 3;
	}
	if (!GunSplicerAnim->Montage_IsPlaying(AttackMontage))
	{
		if (MyDir.Size() < AttackRange)
		{
			//공격 몽타주 실행
			GunSplicerAnim->Montage_Play(AttackMontage, 0.5);
			MyPlayer->HP--;
			auto LookAtPlayer = UKismetMathLibrary::FindLookAtRotation(Me->GetActorLocation(),
														   MyPlayer->GetActorLocation());
			Me->SetActorRotation(LookAtPlayer);
			float RandNum = FMath::RandRange(0,2);
			if (RandNum == 0)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), PlayerHitSound1, 0.75,1,0.35);
			}
			else if(RandNum == 1)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), PlayerHitSound2, 0.75,1,0.08);
			}
			else
			{
				UGameplayStatics::PlaySound2D(GetWorld(), PlayerHitSound3, 0.75,1,0.15);
			}
			GunSplicerState = 2;
		}
		else
		{
			GunSplicerState = 1;
		}
	}
}

void UGunSplicerFSM::TickDying()
{
	if (!GunSplicerAnim->Montage_IsPlaying(DyingMontage))
	{
		GunSplicerAnim->Montage_Play(DyingMontage);
	}
}

void UGunSplicerFSM::DyingStarted(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("GunSplicerDie"))
	{
		Me->Destroy();
	}
}

