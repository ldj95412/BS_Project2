// Fill out your copyright notice in the Description page of Project Settings.


#include "BioshockPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SkeletalMeshComponent.h>

#include "BigDaddyCharacter.h"
#include "BigDaddyFSM.h"
#include "Bullet.h"
# include "BioshockGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "BioShockPlayerAnim.h"
#include "PlayerFSM.h"
#include "Components/PointLightComponent.h"
#include "BioShockPlayerAnim.h"

// Sets default values
ABioshockPlayer::ABioshockPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/KJY/UltimateFPSAnimationsKIT/OnlyHands/JackFPSOnlyHands1.JackFPSOnlyHands1'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(GetMesh());
	springArmComp->SetRelativeLocation(FVector(6.910811f, 0.166791f, 89.999999f));
	springArmComp->TargetArmLength = 0;
	springArmComp->bUsePawnControlRotation = true;

	playerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamComp"));
	playerCamComp->SetupAttachment(springArmComp);
	playerCamComp->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = true;

	
	// 총 스켈레탈메시 컴포넌트 등록
	PistolComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PistolComp"));
	// 부모 컴포넌트를 Mesh 컴포넌트로 설정
	PistolComp->SetupAttachment(GetMesh(), TEXT("L_Socket"));
	//// 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempPistolMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/KJY/UltimateFPSAnimationsKIT/GunModels/SIDEARMS/SK_SIDEARM2.SK_SIDEARM2'"));
	// 데이터 로드 성공했다면 
	if (TempPistolMesh.Succeeded())
	{
		// 스켈레탈메시 데이터 할당
		PistolComp->SetSkeletalMesh(TempPistolMesh.Object);
		// 위치 조정
		PistolComp->SetRelativeLocation(FVector(-0.186406,0.169385,0.080679));
		PistolComp->SetRelativeRotation(FRotator(53.540552,-73.591818,9.280478));
		PistolComp->SetRelativeScale3D(FVector(0.02f));


	}

	// 총 스켈레탈메시 컴포넌트 등록
	ShotgunComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShotgunComp"));
	// 부모 컴포넌트를 Mesh 컴포넌트로 설정
	ShotgunComp->SetupAttachment(GetMesh(), TEXT("L_Socket"));
	// 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempShotGunMesh(TEXT("/ Script / Engine.SkeletalMesh'/Game/KJY/UltimateFPSAnimationsKIT/GunModels/SHOTGUNS/SK_SHOTGUN3.SK_SHOTGUN3'"));
	// 데이터 로드 성공했다면 
	if (TempShotGunMesh.Succeeded())
	{
		// 스켈레탈메시 데이터 할당
		ShotgunComp->SetSkeletalMesh(TempShotGunMesh.Object);
		// 위치 조정
		ShotgunComp->SetRelativeLocation(FVector(-0.293205, 0.378139, 0.033170));
		ShotgunComp->SetRelativeRotation(FRotator(43.289750, -117.600296, 0.467281));
		ShotgunComp->SetRelativeScale3D(FVector(0.02f));
	

	}


}

// Called when the game starts or when spawned
void ABioshockPlayer::BeginPlay()
{
	Super::BeginPlay();
	// PowerPosition = GetMesh()->GetSocketTransform(TEXT("Effect_SuperPower"));

	GunState = 1;
	ShotgunComp->SetRelativeLocation(FVector(0, 0, 5000));
	
}

// Called every frame
void ABioshockPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();

	
}

// Called to bind functionality to input
void ABioshockPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABioshockPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABioshockPlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ABioshockPlayer::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ABioshockPlayer::InputVertical);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ABioshockPlayer::InputJump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ABioshockPlayer::ActionFire);
	PlayerInputComponent->BindAction(TEXT("PrimaryWeapon"), IE_Pressed, this, &ABioshockPlayer::PrimaryWeapon);
	PlayerInputComponent->BindAction(TEXT("SecondaryWeapon"), IE_Pressed, this, &ABioshockPlayer::SecondaryWeapon);
	PlayerInputComponent->BindAction(TEXT("SuperPower"), IE_Pressed, this, &ABioshockPlayer::ActionSuperPower);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ABioshockPlayer::ActionReload);


}

void ABioshockPlayer::Turn(float value)
{
	AddControllerYawInput(value); // 좌우 회전
}

void ABioshockPlayer::LookUp(float value)
{
	AddControllerPitchInput(value); // 상하 회전
}

void ABioshockPlayer::InputHorizontal(float value)
{
	direction.Y = value;

}

void ABioshockPlayer::InputVertical(float value)
{
	direction.X = value;
}

void ABioshockPlayer::InputJump()
{
	Jump();
}

void ABioshockPlayer::Move()
{
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	/*FVector P0 = GetActorLocation();
	FVector vt = direction * walkSpeed * DeltaTime;
	FVector P = P0 + vt;
	SetActorLocation(P);*/
	AddMovementInput(direction);
	direction = FVector::ZeroVector;

}


void ABioshockPlayer::ActionFire()
{

	if (GunState == 1)
	{
		FirePistol();
	}
	else if (GunState == 2)
	{
		FireShotgun();
	}
	else if (GunState == 3)
	{
		FireSuperpower();
	}
}

void ABioshockPlayer::ActionReload()
{
	if (GunState == 1)
	{
		auto anim = Cast<UBioShockPlayerAnim>(GetMesh()->GetAnimInstance());
		anim->PlayPistolReloadAnim();
	}
	else if (GunState == 2)
	{
		auto anim = Cast<UBioShockPlayerAnim>(GetMesh()->GetAnimInstance());
		anim->PlayShotgunReloadAnim();
	}
	else if (GunState == 3)
	{
		
	}
}

void ABioshockPlayer::PrimaryWeapon()
{
	//MyPlayerFSM->mState = EPlayerState::Shotgun;

	//PistolComp->SetVisibility(true);
	//ShotgunComp->SetVisibility(false);

	/*SuperpowerComp->SetVisibility(false);*/
	ShotgunComp->SetRelativeLocation(FVector(0,0,1000));
	PistolComp->SetRelativeLocation(FVector(-0.186406,0.169385,0.080679));

	GunState = 1;
}

void ABioshockPlayer::SecondaryWeapon()
{
	/*PistolComp->SetVisibility(false);
	ShotgunComp->SetVisibility(false);*/
	ShotgunComp->SetRelativeLocation((FVector(-0.272120, 0.121134, 0.078478)));
	PistolComp->SetRelativeLocation(FVector(0, 0, 1000));
	GunState = 2;

}


void ABioshockPlayer::ActionSuperPower()
{
	//MyPlayerFSM->mState = EPlayerState::Superpower;

	//SuperpowerComp->SetVisibility(true);
	/*PistolComp->SetVisibility(false);
	ShotgunComp->SetVisibility(false);*/
	GunState = 3;
	ShotgunComp->SetRelativeLocation((FVector(0,0,1000)));
	PistolComp->SetRelativeLocation(FVector(0, 0, 1000));
}



void ABioshockPlayer::FirePistol()
{
	auto anim = Cast<UBioShockPlayerAnim>(GetMesh()->GetAnimInstance());
	//anim->PlayPistolFireAnim();
	
	

	FTimerHandle GravityTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([this]()->void
		{
			FTransform startPos = PistolComp->GetSocketTransform(TEXT("FirePos"));
			FVector startVec;
			startPos.TransformVector(startVec);
			FVector endPos = playerCamComp->GetComponentLocation() + playerCamComp->GetForwardVector() * 5000;
			FHitResult hitInfo;
			FCollisionQueryParams params;
			params.AddIgnoredActor(this);
			bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startVec, endPos, ECC_Visibility, params);
			if (bHit)
			{
				FTransform bulletTrans;
				bulletTrans.SetLocation(hitInfo.ImpactPoint);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			}
			UKismetSystemLibrary::DrawDebugLine(GetWorld(), startVec, hitInfo.ImpactPoint, FLinearColor::Red, 1, 1);
			ABigDaddyCharacter* BigDaddyHit = Cast<ABigDaddyCharacter>(hitInfo.GetActor());
			if (BigDaddyHit)
			{
				//너 맞았어. 라고 알려주고싶다.
				// enemy->EnemyFSM 이게 더 빠르고 편하긴 하다.
				auto FSM = BigDaddyHit->GetDefaultSubobjectByName(TEXT("EnemyFSM"));

				BigDaddyHit->HP--;

				// UEnemyFSM* enemyFSM = Cast<UEnemyFSM>(FSM);
				// enemyFSM->OnTakeDamage(1);
				BigDaddyHit->MyFSMComponent->ChangeState = 1;
				if (BigDaddyHit->HP == 0)
				{
					BigDaddyHit->MyFSMComponent->RageState = EBigDaddyRageState::Dying;
					BigDaddyHit->HPPntLight->SetLightColor(FLinearColor::Black);
				}
			}

			// TimerHandle 초기화
			
		}), 0.5, false);	// 반복하려면 false를 true로 변경
	GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
	

}


void ABioshockPlayer::FireShotgun()
{
	auto anim = Cast<UBioShockPlayerAnim>(GetMesh()->GetAnimInstance());
	//anim->PlayShotgunFireAnim();

	FTransform startPos = ShotgunComp->GetSocketTransform(TEXT("FirePos"));
	FVector startVec;
	startPos.TransformVector(startVec);
	FVector endPos = playerCamComp->GetComponentLocation() + playerCamComp->GetForwardVector() * 5000;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startVec, endPos, ECC_Visibility, params);
	if (bHit)
	{
		FTransform bulletTrans;
		bulletTrans.SetLocation(hitInfo.ImpactPoint);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

	}
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), startVec, hitInfo.ImpactPoint, FLinearColor::Red, 1, 1);

	ABigDaddyCharacter* BigDaddyHit = Cast<ABigDaddyCharacter>(hitInfo.GetActor());
	if (BigDaddyHit)
	{
		//너 맞았어. 라고 알려주고싶다.
		// enemy->EnemyFSM 이게 더 빠르고 편하긴 하다.
		auto FSM = BigDaddyHit->GetDefaultSubobjectByName(TEXT("EnemyFSM"));

		BigDaddyHit->HP--;

		// UEnemyFSM* enemyFSM = Cast<UEnemyFSM>(FSM);
		// enemyFSM->OnTakeDamage(1);
		BigDaddyHit->MyFSMComponent->ChangeState = 1;
		if (BigDaddyHit->HP == 0)
		{
			BigDaddyHit->MyFSMComponent->RageState = EBigDaddyRageState::Dying;
			BigDaddyHit->HPPntLight->SetLightColor(FLinearColor::Black);
		}
	}
}

void ABioshockPlayer::FireSuperpower()
{
	auto anim = Cast<UBioShockPlayerAnim>(GetMesh()->GetAnimInstance());
	//anim->PlayElectricFireAnim();
	
	//총알(번개)을 생성해서 플레이어 오른손에 배치
	FTransform firePosition = GetActorTransform()+FTransform(FVector(100,0,50)) ;
	// 그위치에서 발사 
	GetWorld()->SpawnActor<ABullet>(BulletFactory, firePosition);

}	
	
