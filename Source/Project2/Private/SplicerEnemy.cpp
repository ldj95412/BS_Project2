// Fill out your copyright notice in the Description page of Project Settings.


#include "SplicerEnemy.h"

#include "BigDaddyCharacter.h"
#include "Bullet.h"
#include "SplicerFSM.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASplicerEnemy::ASplicerEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SplicerMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/LDJ/Resource/Splicer/IdleAnim/Zombie_Idle.Zombie_Idle'"));
    if (SplicerMeshRef.Succeeded())
    {
	    GetMesh()->SetSkeletalMesh(SplicerMeshRef.Object);
    	GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90), FRotator(0,-90,0));
    }

	AttackColl = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxColl"));
	AttackColl->SetupAttachment(GetMesh(),FName("RightHand"));
	AttackColl->SetRelativeLocationAndRotation(FVector(0,-20,0), FRotator(0,90,0));
	AttackColl->SetRelativeScale3D(FVector(0.5));

	MySplicerFSM = CreateDefaultSubobject<USplicerFSM>(TEXT("MyFSM"));
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 0;
}

// Called when the game starts or when spawned
void ASplicerEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this,&ASplicerEnemy::OnBulletCompBeginOverlap);
	
}

// Called every frame
void ASplicerEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASplicerEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASplicerEnemy::OnBulletCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABullet* Bullet = Cast<ABullet>(OtherActor);
	if (!Bullet)
	{
		return;
	}
	SplicerHP--;
	Bullet->Destroy();
}

