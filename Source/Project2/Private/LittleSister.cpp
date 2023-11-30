// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleSister.h"

#include "LittleSisterFSM.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ALittleSister::ALittleSister()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LittleSisterFSM = CreateDefaultSubobject<ULittleSisterFSM>(TEXT("LittleSisterFSM"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/LDJ/Resource/LittleSister/Animation/autorig_actor.autorig_actor'"));
    if (BodyMeshRef.Succeeded())
    {
	    GetMesh()->SetSkeletalMesh(BodyMeshRef.Object);
    	GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90), FRotator(0,-90,0));
    }

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 0;

	MyLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("MyLighting"));

	MyLight->SetLightColor(FLinearColor::Black);
}

// Called when the game starts or when spawned
void ALittleSister::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALittleSister::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALittleSister::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

