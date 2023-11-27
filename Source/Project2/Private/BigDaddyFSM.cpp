// Fill out your copyright notice in the Description page of Project Settings.


#include "BigDaddyFSM.h"
#include "BigDaddyAnimInstance.h"
#include "BigDaddyCharacter.h"
#include "BioshockPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UBigDaddyFSM::UBigDaddyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBigDaddyFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	BigDaddyMe = Cast<ABigDaddyCharacter>(GetOwner());
	BigDaddyAnimInstance = BigDaddyMe->GetMesh()->GetAnimInstance();
	BigDaddyAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UBigDaddyFSM::JumpStarted);
	BigDaddyAnimInstance->OnMontageEnded.AddDynamic(this, &UBigDaddyFSM::JumpEnded);
	BigDaddyAnimInstance->OnMontageStarted.AddDynamic(this, &UBigDaddyFSM::Jeet1Started);
	BigDaddyAnimInstance->OnMontageEnded.AddDynamic(this, &UBigDaddyFSM::Jeet1Ended);
	BigDaddyAnimInstance->OnMontageStarted.AddDynamic(this, &UBigDaddyFSM::Jeet2Started);
	BigDaddyAnimInstance->OnMontageEnded.AddDynamic(this, &UBigDaddyFSM::Jeet2Ended);

	BigDaddyAnimInstance->OnMontageStarted.AddDynamic(this, &UBigDaddyFSM::Spear1Started);
	BigDaddyAnimInstance->OnMontageEnded.AddDynamic(this, &UBigDaddyFSM::Spear1Ended);
	BigDaddyAnimInstance->OnMontageStarted.AddDynamic(this, &UBigDaddyFSM::Spear2Started);
	BigDaddyAnimInstance->OnMontageEnded.AddDynamic(this, &UBigDaddyFSM::Spear2Ended);
	BigDaddyAnimInstance->OnMontageStarted.AddDynamic(this, &UBigDaddyFSM::Spear3Started);
	BigDaddyAnimInstance->OnMontageEnded.AddDynamic(this, &UBigDaddyFSM::Spear3Ended);

	BigDaddyAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UBigDaddyFSM::AttackStarted);
	BigDaddyAnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &UBigDaddyFSM::AttackEnded);
}


// Called every frame
void UBigDaddyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (ChangeState == 0)
	{
		switch (NormalState)
		{
		case EBigDaddyNormalState::Idle:
			TickIdle();
			break;

		case EBigDaddyNormalState::Walk:
			TickWalk();
			break;

		case EBigDaddyNormalState::EyesOn:
			TickEyesOn();
			break;

		case EBigDaddyNormalState::Pushing:
			TickPushing();
			break;
		}
	}

	else if (ChangeState == 1)
	{
		switch (RageState)
		{
		case EBigDaddyRageState::Roaring:
			TickRoaring();
			break;

		case EBigDaddyRageState::Run:
			TickRun();
			break;

		case EBigDaddyRageState::Attack:
			TickAttack();
			break;

		case EBigDaddyRageState::Dying:
			TickDying();
			break;
		}
	}
}

void UBigDaddyFSM::TickIdle()
{
	PlayerTarget = Cast<ABioshockPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	FVector MyDir = PlayerTarget->GetActorLocation() - BigDaddyMe->GetActorLocation();
	if (PlayerTarget && MyDir.Size() < EyeSightRange + 1000)
	{
		NormalState = EBigDaddyNormalState::EyesOn;
	}
}

void UBigDaddyFSM::TickWalk()
{
	FVector MyDir = PlayerTarget->GetActorLocation() - BigDaddyMe->GetActorLocation();
	DirectionRotate = FMath::RadiansToDegrees(FMath::Atan2(MyDir.Y, MyDir.X));
	BigDaddyMe->AddMovementInput(MyDir.GetSafeNormal());
	BigDaddyMe->GetCharacterMovement()->MaxWalkSpeed = 250;
	if (MyDir.Size() <= MeleeAttackRange)
	{
		NormalState = EBigDaddyNormalState::Pushing;
	}
}

void UBigDaddyFSM::TickEyesOn()
{
	FVector MyDir = PlayerTarget->GetActorLocation() - BigDaddyMe->GetActorLocation();
	auto Distancing = FVector::Distance(PlayerTarget->GetActorLocation(), BigDaddyMe->GetActorLocation());
	DirectionRotate = FMath::RadiansToDegrees(FMath::Atan2(MyDir.Y, MyDir.X));
	BigDaddyMe->SetActorRotation(FRotator(0, DirectionRotate, 0));
	BigDaddyMe->GetCharacterMovement()->MaxWalkSpeed = 0;
	if (Distancing < EyeSightRange)
	{
		NormalState = EBigDaddyNormalState::Walk;
	}
}

void UBigDaddyFSM::TickPushing()
{
	FVector MyDir = PlayerTarget->GetActorLocation() - BigDaddyMe->GetActorLocation();
	if (!DoOncePushing)
	{
		BigDaddyMe->GetCharacterMovement()->MaxWalkSpeed = 0;
		BigDaddyAnimInstance->Montage_Play(PushingAction);
		DoOncePushing++;
	}

	CurrentTime = CurrentTime + GetWorld()->GetDeltaSeconds();
	if (CurrentTime > 5 && MyDir.Size() > MeleeAttackRange)
	{
		NormalState = EBigDaddyNormalState::Walk;
		DoOncePushing = 0;
		CurrentTime = 0;
	}
}

void UBigDaddyFSM::TickRoaring()
{
	FVector MyDir = PlayerTarget->GetActorLocation() - BigDaddyMe->GetActorLocation();
	FSMCurrentTime = FSMCurrentTime + GetWorld()->GetDeltaSeconds();
	if (!DoOnceRoaring)
	{
		BigDaddyMe->GetCharacterMovement()->MaxWalkSpeed = 0;
		BigDaddyAnimInstance->Montage_Play(RoaringAction, 1.5);
		DoOnceRoaring++;
	}
	if (DoOnceRoaring == 1 && FSMCurrentTime > 1)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShaking, BigDaddyMe->GetActorLocation(), 100000,
		                                       100000);
		UGameplayStatics::PlaySound2D(GetWorld(), RoarSound, 8);
		DoOnceRoaring++;
	}
	if (!BigDaddyAnimInstance->Montage_IsPlaying(RoaringAction))
	{
		DoOnceRoaring = 0;
		FSMCurrentTime = 0;
		RageState = EBigDaddyRageState::Run;
	}
}

void UBigDaddyFSM::TickRun()
{
	FVector MyDir = PlayerTarget->GetActorLocation() - BigDaddyMe->GetActorLocation();
	DirectionRotate = FMath::RadiansToDegrees(FMath::Atan2(MyDir.Y, MyDir.X));
	BigDaddyMe->AddMovementInput(MyDir.GetSafeNormal());
	BigDaddyMe->GetCharacterMovement()->MaxWalkSpeed = 600;

	if (MyDir.Size() <= LongRange)
	{
		RageState = EBigDaddyRageState::Attack;
	}
}

void UBigDaddyFSM::TickAttack()
{
	FVector MyDir = PlayerTarget->GetActorLocation() - BigDaddyMe->GetActorLocation();
	FTimerHandle Handle;




	
	// FHitResult hitResult;
	// FVector start = BigDaddyMe->GetActorLocation() + FVector(0,0,90);
	// FVector end = start + BigDaddyMe->GetActorForwardVector() * 800;
	// FCollisionQueryParams params;
	// if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility))
	// {
	// 	auto hitComp = hitResult.GetComponent();
	// 	auto temp = hitResult.Distance;
	// }
	// else
	// {
	// 	// 부딪힌 곳이 없다... => 허공
	// }




	
	if (!DoOnceAttack)
	{
		BigDaddyMe->GetCharacterMovement()->MaxWalkSpeed = 0;

		int32 AttackRandNum;
		
		if (MyDir.Size() <= MeleeAttackRange)
		{
			AttackRandNum = FMath::RandRange(0, 2);
		}
		else if(MyDir.Size() <= MiddleRange && MyDir.Size() > MeleeAttackRange)
		{
			AttackRandNum = FMath::RandRange(3, 4);
		}
		else if(MyDir.Size() <= LongRange && MyDir.Size() > MiddleRange)
		{
			AttackRandNum = FMath::RandRange(5, 7);
		}
			
		if (AttackRandNum == 0)
		{
			BigDaddyAnimInstance->Montage_Play(AttackActionPunch);
		}

		else if (AttackRandNum == 1)
		{
			BigDaddyAnimInstance->Montage_Play(SpearAttack2);
		}

		else if (AttackRandNum == 2)
		{
			BigDaddyAnimInstance->Montage_Play(AttackActionSweep);
		}

		else if (AttackRandNum == 3)
		{
			// BigDaddyAnimInstance->Montage_Play(AttackActionJump);
			BigDaddyMe->PlayAnimMontage(AttackActionJump);
			UGameplayStatics::PlaySound2D(GetWorld(), AtkJumpSound, 2, 1, 0.2);
			GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([this]()-> void
			{
				UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShaking, BigDaddyMe->GetActorLocation(), 5000,
													   5000);
			}), 1.7, false);
		}

		else if (AttackRandNum == 4)
		{
			BigDaddyAnimInstance->Montage_Play(AttackJeet1);
		}

		else if (AttackRandNum == 5)
		{
			BigDaddyAnimInstance->Montage_Play(SpearAttack1);
		}

		else if (AttackRandNum == 6)
		{
			BigDaddyAnimInstance->Montage_Play(AttackJeet2);
		}

		else if (AttackRandNum == 7)
		{
			BigDaddyAnimInstance->Montage_Play(SpearAttack3);
		}
		DoOnceAttack++;
	}

	if (!BigDaddyAnimInstance->IsAnyMontagePlaying())
	{
		DoOnceAttack = 0;
		if (MyDir.Size() <= LongRange)
		{
			auto LookAtPlayer = UKismetMathLibrary::FindLookAtRotation(BigDaddyMe->GetActorLocation(),
			                                                           PlayerTarget->GetActorLocation());
			BigDaddyMe->SetActorRotation(LookAtPlayer);
			RageState = EBigDaddyRageState::Attack;
		}
		else
		{
			RageState = EBigDaddyRageState::Run;
		}
	}
}

void UBigDaddyFSM::TickDying()
{
	FVector MyDir = PlayerTarget->GetActorLocation() - BigDaddyMe->GetActorLocation();
	if (!DoOnceDying)
	{
		BigDaddyMe->GetCharacterMovement()->MaxWalkSpeed = 0;
		auto AttackRandNum = FMath::RandRange(0, 2);
		BigDaddyAnimInstance->Montage_Play(DyingAction);
		DoOnceDying++;
	}
	FSMCurrentTime = FSMCurrentTime + GetWorld()->GetDeltaSeconds();
	if (FSMCurrentTime > 3)
	{
		BigDaddyAnimInstance->Montage_Pause(DyingAction);
	}
}


// 이 이후 부터는 애니메이션 스무스 처리

void UBigDaddyFSM::JumpStarted(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("JumpLandNotify"))
	{
		AnimStartLocation = BigDaddyMe->GetActorLocation();
	}
}

void UBigDaddyFSM::JumpEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage && !bInterrupted)
	{
		if (Montage == AttackActionJump)
		{
			AnimEndLocation = AnimStartLocation + (BigDaddyMe->GetActorForwardVector() * 235.65) - (BigDaddyMe->
				GetActorRightVector() * 19.65);
			BigDaddyMe->SetActorLocation(AnimEndLocation);
		}
	}
}

void UBigDaddyFSM::Jeet1Started(UAnimMontage* Montage)
{
	if (Montage == AttackJeet1)
	{
		AnimStartLocation = BigDaddyMe->GetActorLocation();
	}
}

void UBigDaddyFSM::Jeet1Ended(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage && !bInterrupted)
	{
		if (Montage == AttackJeet1)
		{
			AnimEndLocation = AnimStartLocation + (BigDaddyMe->GetActorForwardVector() * 448.04) - (BigDaddyMe->
				GetActorRightVector() * 9.2);
			BigDaddyMe->SetActorLocation(AnimEndLocation);
		}
	}
}

void UBigDaddyFSM::Jeet2Started(UAnimMontage* Montage)
{
	if (Montage == AttackJeet2)
	{
		AnimStartLocation = BigDaddyMe->GetActorLocation();
	}
}

void UBigDaddyFSM::Jeet2Ended(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage && !bInterrupted)
	{
		if (Montage == AttackJeet2)
		{
			AnimEndLocation = AnimStartLocation + (BigDaddyMe->GetActorForwardVector() * 773.7) - (BigDaddyMe->
				GetActorRightVector() * 23.775);
			BigDaddyMe->SetActorLocation(AnimEndLocation);
		}
	}
}

void UBigDaddyFSM::Spear1Started(UAnimMontage* Montage)
{
	if (Montage == SpearAttack1)
	{
		AnimStartLocation = BigDaddyMe->GetActorLocation();
	}
}

void UBigDaddyFSM::Spear1Ended(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage && !bInterrupted)
	{
		if (Montage == SpearAttack1)
		{
			AnimEndLocation = AnimStartLocation + (BigDaddyMe->GetActorForwardVector() * 448);
			BigDaddyMe->SetActorLocation(AnimEndLocation);
		}
	}
}

void UBigDaddyFSM::Spear2Started(UAnimMontage* Montage)
{
	if (Montage == SpearAttack2)
	{
		AnimStartLocation = BigDaddyMe->GetActorLocation();
	}
}

void UBigDaddyFSM::Spear2Ended(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage && !bInterrupted)
	{
		if (Montage == SpearAttack2)
		{
			AnimEndLocation = AnimStartLocation + (BigDaddyMe->GetActorForwardVector() * 189);
			BigDaddyMe->SetActorLocation(AnimEndLocation);
		}
	}
}

void UBigDaddyFSM::Spear3Started(UAnimMontage* Montage)
{
	if (Montage == SpearAttack3)
	{
		AnimStartLocation = BigDaddyMe->GetActorLocation();
	}
}

void UBigDaddyFSM::Spear3Ended(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage && !bInterrupted)
	{
		if (Montage == SpearAttack3)
		{
			AnimEndLocation = AnimStartLocation + (BigDaddyMe->GetActorForwardVector() * 542.8);
			BigDaddyMe->SetActorLocation(AnimEndLocation);
		}
	}
}

void UBigDaddyFSM::AttackStarted(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("Attack"))
	{
		BigDaddyMe->AttackCollision->SetGenerateOverlapEvents(true);
		FRotator AttackCollRotator = BigDaddyMe->AttackCollision->GetComponentRotation();
		UKismetSystemLibrary::DrawDebugCapsule(GetWorld(),
		                                       BigDaddyMe->AttackCollision->GetComponentLocation(),
		                                       BigDaddyMe->AttackCollision->GetScaledCapsuleHalfHeight(),
		                                       BigDaddyMe->AttackCollision->GetScaledCapsuleRadius(),
		                                       AttackCollRotator,
		                                       FLinearColor::Green,
		                                       2,
		                                       1);
	}
}

void UBigDaddyFSM::AttackEnded(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName("Attack"))
	{
		BigDaddyMe->AttackCollision->SetGenerateOverlapEvents(false);
		FRotator AttackCollRotator = BigDaddyMe->AttackCollision->GetComponentRotation();
		UKismetSystemLibrary::DrawDebugCapsule(GetWorld(),
		                                       BigDaddyMe->AttackCollision->GetComponentLocation(),
		                                       BigDaddyMe->AttackCollision->GetScaledCapsuleHalfHeight(),
		                                       BigDaddyMe->AttackCollision->GetScaledCapsuleRadius(),
		                                       AttackCollRotator,
		                                       FLinearColor::Red,
		                                       2,
		                                       1);
	}
}
