// Fill out your copyright notice in the Description page of Project Settings.


#include "GunSplicerEnemy.h"

#include "Bullet.h"
#include "GunSplicerFSM.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGunSplicerEnemy::AGunSplicerEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/LDJ/Resource/GunSplicer/Zombie_Idle.Zombie_Idle'"));
    if (BodyMeshRef.Succeeded())
    {
	    GetMesh()->SetSkeletalMesh(BodyMeshRef.Object);
    	GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90), FRotator(0,-90,0));
    }

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(GetMesh(), FName("RightHand"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GunMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/LDJ/Resource/GunSplicer/NewFolder1/HandGun2.HandGun2'"));
	if (GunMeshRef.Succeeded())
	{
		GunMesh->SetStaticMesh(GunMeshRef.Object);
		GunMesh->SetRelativeLocationAndRotation(FVector(9.069018,-16.605398,4.619944), FRotator(72.396561,131.920934,204.010674));
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 0;

	GunSplicerFSM = CreateDefaultSubobject<UGunSplicerFSM>(TEXT("GunSplicerFSM"));
	GetCapsuleComponent()->SetCollisionProfileName(FName("Pawn"));
	GetMesh()->SetCollisionProfileName(FName("Enemy"));
}

void AGunSplicerEnemy::OnBulletCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABullet* Bullet = Cast<ABullet>(OtherActor);
	if (!Bullet)
	{
		return;
	}
	GunSplicerHP--;
	Bullet->Destroy();
	if (GunSplicerHP == 0)
	{
		GunSplicerFSM->GunSplicerState = 3;
	}
}

// Called when the game starts or when spawned
void AGunSplicerEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this,&AGunSplicerEnemy::OnBulletCompBeginOverlap);
	
}

// Called every frame
void AGunSplicerEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGunSplicerEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

