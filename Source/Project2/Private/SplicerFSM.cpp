// Fill out your copyright notice in the Description page of Project Settings.


#include "SplicerFSM.h"

#include "BioshockPlayer.h"
#include "SplicerEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USplicerFSM::USplicerFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USplicerFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	MyPlayer = Cast<ABioshockPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	Me = Cast<ASplicerEnemy>(GetOwner());
	SplicerAnim = Me->GetMesh()->GetAnimInstance();

	SplicerAnim->OnPlayMontageNotifyBegin.AddDynamic(this, &USplicerFSM::MeleeDyingStarted);
	SplicerAnim->OnPlayMontageNotifyBegin.AddDynamic(this, &USplicerFSM::TrueDyingStarted);
}


// Called every frame
void USplicerFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (SplicerState == 0) //정지 - 경계 모드
	{
		TickIdle();
	}
	else if (SplicerState == 1) //걷기 모드
	{
		TickWalk();
	}
	else if (SplicerState == 2) //뛰기 모드
	{
		TickRun();
	}
	else if (SplicerState == 3) //공격 모드
	{
		TickAttack();
	}
	else if (SplicerState == 4) //사망 모드
	{
		TickDying();
	}
	else if (SplicerState == 5) //발악 이동 모드
	{
		TickCrawl();
	}
	else if (SplicerState == 6) //발악 공격 모드
	{
		TickCrawlAttack();
	}
	else if (SplicerState == 7) //찐 사망 모드
	{
		TickTrueDie();
	}
}

void USplicerFSM::TickIdle()
{
	FVector MyDir = MyPlayer->GetActorLocation() - Me->GetActorLocation();
	if (MyPlayer && MyDir.Size() < DetectingRange)
	{
		SplicerState = 1; 
	}
	if (Me->SplicerHP < 10)
	{
		SplicerState = 2; 
	}
}

void USplicerFSM::TickWalk()
{
	Me->GetCharacterMovement()->MaxWalkSpeed = 250;
	FVector MyDir = MyPlayer->GetActorLocation() - Me->GetActorLocation();
	Me->AddMovementInput(MyDir.GetSafeNormal());
	if (MyDir.Size() < AttackRange && Me->SplicerHP > 5)
	{
		SplicerState = 3;
	}
	if (Me->SplicerHP < 10 && Me->SplicerHP > 5)
	{
		SplicerState = 2;
	}
	else if(Me->SplicerHP <= 5)
	{
		SplicerState = 4;
	}
}

void USplicerFSM::TickRun()
{
	Me->GetCharacterMovement()->MaxWalkSpeed = 500;
	FVector MyDir = MyPlayer->GetActorLocation() - Me->GetActorLocation();
	Me->AddMovementInput(MyDir.GetSafeNormal());
	if (MyDir.Size() < AttackRange && Me->SplicerHP > 5)
	{
		SplicerState = 3;
	}
	if (Me->SplicerHP < 5)
	{
		SplicerState = 4;
	}
}

void USplicerFSM::TickAttack()
{
	Me->GetCharacterMovement()->MaxWalkSpeed = 0;
	FVector MyDir = MyPlayer->GetActorLocation() - Me->GetActorLocation();
	if (!SplicerAnim->Montage_IsPlaying(AttackMontage))
	{
		if (MyDir.Size() < AttackRange)
		{
			//공격 몽타주 실행
			SplicerAnim->Montage_Play(AttackMontage , 2);
			MyPlayer->HP--;
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
			SplicerState = 3;
		}
		else if(MyDir.Size() > AttackRange)
		{
			SplicerState = 2;
		}
		if (Me->SplicerHP < 6)
		{
			SplicerState = 4;
		}
	}
}

void USplicerFSM::TickDying()
{
	Me->SplicerHP = 5;
	if (!SplicerAnim->Montage_IsPlaying(DyingMontage))
	{
		SplicerAnim->Montage_Play(DyingMontage);
		//진행중이면 노티파이에 의해 상태 5로 넘어감
	}
}

void USplicerFSM::TickCrawl()
{
	Me->GetCharacterMovement()->MaxWalkSpeed = 150;
	FVector MyDir = MyPlayer->GetActorLocation() - Me->GetActorLocation();
	Me->AddMovementInput(MyDir.GetSafeNormal());
	if (MyDir.Size() < AttackRange && Me->SplicerHP < 6 && Me->SplicerHP > 0)
	{
		SplicerState = 6;
	}
	if (Me->SplicerHP < 1)
	{
		SplicerState = 7;
	}
}

void USplicerFSM::TickCrawlAttack()
{
	Me->GetCharacterMovement()->MaxWalkSpeed = 0;
	FVector MyDir = MyPlayer->GetActorLocation() - Me->GetActorLocation();
	if (!SplicerAnim->Montage_IsPlaying(CrawlAttackMontage))
	{
		SplicerAnim->Montage_Play(CrawlAttackMontage, 2);
		MyPlayer->HP--;
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
	}
	else
	{
		if (MyDir.Size() < AttackRange && Me->SplicerHP < 6 && Me->SplicerHP > 0)
		{
			SplicerState = 6;
		}
		else if(MyDir.Size() >= AttackRange && Me->SplicerHP < 6)
		{
			SplicerState = 5;
		}
	}
	if (Me->SplicerHP < 1)
	{
		SplicerState = 7;
	}

}

void USplicerFSM::TickTrueDie()
{
	//내려가는 몽타주
	Me->GetCharacterMovement()->MaxWalkSpeed = 0;
	if (!SplicerAnim->Montage_IsPlaying(TrueDyingMontage))
	{
		SplicerAnim->Montage_Play(TrueDyingMontage);
		//진행중이면 노티파이에 의해 상태 5로 넘어감
	}
}

void USplicerFSM::MeleeDyingStarted(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("CrawlMode"))
	{
		SplicerState = 5;
	}
}

void USplicerFSM::TrueDyingStarted(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("TrueDie"))
	{
		Me->Destroy();
	}
}
