// Fill out your copyright notice in the Description page of Project Settings.


#include "BigDaddyCharacter.h"
#include "BigDaddyFSM.h"
#include "BioshockPlayer.h"
#include "Bullet.h"
#include "Components/CapsuleComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/RectLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABigDaddyCharacter::ABigDaddyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* 컴포넌트 생성 */
	MyFSMComponent = CreateDefaultSubobject<UBigDaddyFSM>(TEXT("BigDaddyFSM"));
	AttackCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackColl"));
	AttackCollision->SetupAttachment(GetMesh(),FName("hand_r"));
	DrillHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drill"));
	DrillHand->SetupAttachment(GetMesh(),TEXT("hand_r"));
	BigDaddySpotLight = CreateDefaultSubobject<URectLightComponent>(TEXT("BigDaddyLight")); 
	/* 에셋 불러오기 */
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/LDJ/Resource/BigDaddy/Animation/Idle/Mutant_Breathing_Idle_Main.Mutant_Breathing_Idle_Main'"));
	if (SkelMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkelMeshRef.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimClassRef(TEXT("/Game/LDJ/BP/ABP_BigDaddy.ABP_BigDaddy_C"));
	if (AnimClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimClassRef.Class);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DrillMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/LDJ/Resource/NewBigDaddy/NewMesh/DrillSocket3.DrillSocket3'"));
    if (DrillMeshRef.Succeeded())
    {
	    DrillHand->SetStaticMesh(DrillMeshRef.Object);
    	DrillHand->SetRelativeLocationAndRotation(FVector(-0.072722,0.032629,0.008914),FRotator(289.399169,7.200028,-0.000000));
    	DrillHand->SetRelativeScale3D(FVector(0.015f));
    }
	
	/* 디테일 수정 */
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.5));
	GetCapsuleComponent()->SetCapsuleRadius(50);
	GetCapsuleComponent()->SetCapsuleHalfHeight(80);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-77), FRotator(0,270,0));

	AttackCollision->SetGenerateOverlapEvents(false);
	AttackCollision->SetCapsuleRadius(0.2);
	AttackCollision->SetCapsuleHalfHeight(0.7);
	AttackCollision->SetRelativeLocationAndRotation(FVector(0.032414,-0.476480,-0.084016),FRotator(1.727941,9.851076,80.148923));
	AttackCollision->SetCollisionProfileName(FName("Enemy"));
	
	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	BigDaddySpotLight->SetRelativeLocation(FVector(0,0,200));
	BigDaddySpotLight->SetRelativeRotation(FRotator(-90,0,0));
}

// Called when the game starts or when spawned
void ABigDaddyCharacter::BeginPlay()
{
	Super::BeginPlay();
	/* 오버랩 이벤트 생성 */
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this,&ABigDaddyCharacter::OnBulletCompBeginOverlap);
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this,&ABigDaddyCharacter::OnAttackBeginOverlap);
}

// Called every frame
void ABigDaddyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABigDaddyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/* 오버랩 이벤트 생성 */
void ABigDaddyCharacter::OnBulletCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABullet* Bullet = Cast<ABullet>(OtherActor);
	if (!Bullet)
	{
		return;
	}
	MyFSMComponent->ChangeState = 1;
	HP--;
	Bullet->Destroy();
	if (!MyFSMComponent->BigDaddyAnimInstance->Montage_IsPlaying(MyFSMComponent->ElectricStun) && HP > 0)
	{
		MyFSMComponent->BigDaddyAnimInstance->Montage_Stop(0.25);
		MyFSMComponent->BigDaddyAnimInstance->Montage_Play(MyFSMComponent->ElectricStun);
	}

	if (HP == 0)
	{
		MyFSMComponent->RageState = EBigDaddyRageState::Dying;
	}
}

void ABigDaddyCharacter::OnAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Player = Cast<ABioshockPlayer>(OtherActor);
	if (!Player)
	{
		return;
	}
	AttackCollision->SetGenerateOverlapEvents(false);
	UGameplayStatics::PlaySound2D(GetWorld(), AtkPunchSound, 6,1,0.15);
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
	UKismetSystemLibrary::DrawDebugCapsule(GetWorld(),
	this->AttackCollision->GetComponentLocation(),
	this->AttackCollision->GetScaledCapsuleHalfHeight(),
	this->AttackCollision->GetScaledCapsuleRadius(),
	this->AttackCollision->GetComponentRotation(),
	FLinearColor::Yellow,
	2,
	1);
}