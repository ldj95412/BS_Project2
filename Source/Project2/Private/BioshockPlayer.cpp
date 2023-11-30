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
#include "WeaponAnim.h"

// Sets default values
ABioshockPlayer::ABioshockPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(6.910811f, 0.166791f, 89.999999f));
	springArmComp->TargetArmLength = 0;
	springArmComp->bUsePawnControlRotation = true;

	playerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamComp"));
	playerCamComp->SetupAttachment(springArmComp);
	playerCamComp->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = true;

	GetMesh()->SetupAttachment(playerCamComp);
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/KJY/UltimateFPSAnimationsKIT/Demo/PlayerMannequin/Mesh/Arms.Arms'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// 총 스켈레탈메시 컴포넌트 등록
	ShotgunComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShotgunComp"));
	// 부모 컴포넌트를 Mesh 컴포넌트로 설정
	ShotgunComp->SetupAttachment(GetMesh(), TEXT("R_Socket"));
	// 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempShotGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/KJY/UltimateFPSAnimationsKIT/GunModels/SHOTGUNS/SK_SHOTGUN3.SK_SHOTGUN3'"));
	// 데이터 로드 성공했다면 
	if (TempShotGunMesh.Succeeded())
	{
		// 스켈레탈메시 데이터 할당
		ShotgunComp->SetSkeletalMesh(TempShotGunMesh.Object);
		// 위치 조정
		ShotgunComp->SetRelativeLocation(FVector(7.170502, 1.062384, 6.651627));
		ShotgunComp->SetRelativeRotation(FRotator(-0.000000, 83.544888, -3.663642));
		ShotgunComp->SetRelativeScale3D(FVector(1.0f));
	}

	// 총 스켈레탈메시 컴포넌트 등록
	KnifeComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("KnifeComp"));
	// 부모 컴포넌트를 Mesh 컴포넌트로 설정
	KnifeComp->SetupAttachment(GetMesh(), TEXT("R_Socket"));
	// 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempKnifeMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/KJY/UltimateFPSAnimationsKIT/GunModels/MELEE/SK_knife.SK_knife'"));
	// 데이터 로드 성공했다면 
	if (TempKnifeMesh.Succeeded())
	{
		// 스켈레탈메시 데이터 할당
		KnifeComp->SetSkeletalMesh(TempKnifeMesh.Object);
		// 위치 조정
		KnifeComp->SetRelativeLocation(FVector(7.082877, -4.110451, -0.771220));
		KnifeComp->SetRelativeRotation(FRotator(-5.994066, 610.933196, 344.084205));
		KnifeComp->SetRelativeScale3D(FVector(1.0f));
	}

	// 총 스켈레탈메시 컴포넌트 등록
	SubgunComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SubgunComp"));
	// 부모 컴포넌트를 Mesh 컴포넌트로 설정
	SubgunComp->SetupAttachment(GetMesh(), TEXT("R_Socket"));
	// 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSubgunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/KJY/UltimateFPSAnimationsKIT/GunModels/SUBMACHINEGUNS/SK_SUBMACHINEGUN4.SK_SUBMACHINEGUN4'"));
	// 데이터 로드 성공했다면 
	if (TempSubgunMesh.Succeeded())
	{
		// 스켈레탈메시 데이터 할당
		SubgunComp->SetSkeletalMesh(TempSubgunMesh.Object);
		// 위치 조정
		SubgunComp->SetRelativeLocation(FVector(2.275763, -2.349950, 7.232812));
		SubgunComp->SetRelativeRotation(FRotator(6.031903, 68.297558, -351.781684));
		SubgunComp->SetRelativeScale3D(FVector(1.0f));
	}
}

// Called when the game starts or when spawned
void ABioshockPlayer::BeginPlay()
{
	Super::BeginPlay();
	// PowerPosition = GetMesh()->GetSocketTransform(TEXT("Effect_SuperPower"));

	GunState = 1;
	
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
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ABioshockPlayer::FireReleased);

	PlayerInputComponent->BindAction(TEXT("PrimaryWeapon"), IE_Pressed, this, &ABioshockPlayer::PrimaryWeapon);
	PlayerInputComponent->BindAction(TEXT("SecondaryWeapon"), IE_Pressed, this, &ABioshockPlayer::SecondaryWeapon);
	PlayerInputComponent->BindAction(TEXT("ThridWeapon"), IE_Pressed, this, &ABioshockPlayer::ThridWeapon);
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
		FireShotgun();
	}
	else if (GunState == 2)
	{
		WieldKnife();
	}
	else if (GunState == 3)
	{
		FireSubgun();
	}
}

void ABioshockPlayer::FireReleased()
{
	GetWorld()->GetTimerManager().PauseTimer(Handle);
}

void ABioshockPlayer::ActionReload()
{
	if (GunState == 1)
	{
		auto anim = Cast<UBioShockPlayerAnim>(GetMesh()->GetAnimInstance());
		anim->PlayShotgunArmReloadAnim();

		auto animGun = Cast<UWeaponAnim>(ShotgunComp->GetAnimInstance());
		animGun->PlayShotgunReloadAnim();

	}
	else if (GunState == 3)
	{
		auto anim = Cast<UBioShockPlayerAnim>(GetMesh()->GetAnimInstance());
		anim->PlaySubgunArmReloadAnim();

		//auto animGun = Cast<UWeaponAnim>(GetMesh()->GetAnimInstance());
		//animGun->PlaySubgunReloadAnim();
	}
}

void ABioshockPlayer::PrimaryWeapon()
{
	ShotgunComp->SetVisibility(true);
	KnifeComp->SetVisibility(false);
	SubgunComp->SetVisibility(false);

	//ShotgunComp->SetRelativeLocation(FVector(0,0,1000));

	GunState = 1;
}

void ABioshockPlayer::SecondaryWeapon()
{
	
	ShotgunComp->SetVisibility(false);
	KnifeComp->SetVisibility(true);
	SubgunComp->SetVisibility(false);

	/*ShotgunComp->SetRelativeLocation((FVector(-0.272120, 0.121134, 0.078478)));*/
	
	GunState = 2;

}

void ABioshockPlayer::ThridWeapon()
{
	ShotgunComp->SetVisibility(false);
	KnifeComp->SetVisibility(false);
	SubgunComp->SetVisibility(true);

	GunState = 3;

	//ShotgunComp->SetRelativeLocation((FVector(0,0,1000)));
}


void ABioshockPlayer::FireShotgun()
{
	auto controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(CameraShakeShotgun);

	auto anim = Cast<UBioShockPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->PlayShotgunFireAnim();

	
	//총알(번개)을 생성해서 플레이어 오른손에 배치
	FTransform firePosition;
	firePosition.SetLocation(GetActorLocation()+GetActorForwardVector()*100+GetActorUpVector()*50);
	firePosition.SetRotation(GetActorRotation().Quaternion());
	// 그위치에서 발사 
	GetWorld()->SpawnActor<ABullet>(BulletFactory, firePosition);


}

void ABioshockPlayer::WieldKnife()
{
	auto anim = Cast<UBioShockPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->PlayKnifeWieldAnim();
}

void ABioshockPlayer::FireSubgun()
{
	auto controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(CameraShakeSubgun);

	auto anim = Cast<UBioShockPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->PlaySubgunFireAnim();

	FVector startPos = playerCamComp->GetComponentLocation();
	FVector endPos = playerCamComp->GetComponentLocation() + playerCamComp->GetForwardVector() * 5000;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
	if (bHit)
	{
		FTransform bulletTrans;
		bulletTrans.SetLocation(hitInfo.ImpactPoint);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

	}
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), startPos, hitInfo.ImpactPoint, FLinearColor::Red, 1, 1);

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

	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([this]()-> void
		{
			auto controller = GetWorld()->GetFirstPlayerController();
			controller->PlayerCameraManager->StartCameraShake(CameraShakeSubgun);


			auto anim = Cast<UBioShockPlayerAnim>(GetMesh()->GetAnimInstance());
			anim->PlaySubgunFireAnim(2);

			FVector startPos = playerCamComp->GetComponentLocation();
			FVector endPos = playerCamComp->GetComponentLocation() + playerCamComp->GetForwardVector() * 5000;
			FHitResult hitInfo;
			FCollisionQueryParams params;
			params.AddIgnoredActor(this);
			bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
			if (bHit)
			{
				FTransform bulletTrans;
				bulletTrans.SetLocation(hitInfo.ImpactPoint);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			}
			UKismetSystemLibrary::DrawDebugLine(GetWorld(), startPos, hitInfo.ImpactPoint, FLinearColor::Red, 1, 1);

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
		}), 0.2, true);	
}


	
