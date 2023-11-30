// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleSisterFSM.h"

#include "BigDaddyCharacter.h"
#include "BioshockPlayer.h"
#include "LittleSister.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULittleSisterFSM::ULittleSisterFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULittleSisterFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Me = Cast<ALittleSister>(GetOwner());
	Target = Cast<ABioshockPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	BigDaddy = Cast<ABigDaddyCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), BigDaddyFactory));
}


// Called every frame
void ULittleSisterFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsValid(BigDaddy) && BigDaddy->HP < 1)
	{
		LittleSisterState = 2;
		UE_LOG(LogTemp,Warning,TEXT("TEST"));
	}
	if(LittleSisterState == 0)
	{
		TickIdle();
		UE_LOG(LogTemp,Warning,TEXT("TickIdle"));
	}
	else if(LittleSisterState == 1)
	{
		TickEscape();
		UE_LOG(LogTemp,Warning,TEXT("TickEscape"));
	}
	else if(LittleSisterState == 2)
	{
		TickGiveUp();
		UE_LOG(LogTemp,Warning,TEXT("TickGiveUp"));
	}
	// ...
}

void ULittleSisterFSM::TickIdle()
{
	Me->GetCharacterMovement()->MaxWalkSpeed = 0;
	auto temp = UKismetMathLibrary::FindLookAtRotation(Me->GetActorLocation(), Target->GetActorLocation());
	Me->SetActorRelativeRotation(temp);
	FVector MyDir = Target->GetActorLocation() - Me->GetActorLocation();
	if (Target && MyDir.Size() < EscapeRange)
	{
		LittleSisterState = 1; 
	}
}

void ULittleSisterFSM::TickEscape()
{
	Me->GetCharacterMovement()->MaxWalkSpeed = 150;
	auto temp = UKismetMathLibrary::FindLookAtRotation(Me->GetActorLocation(), Target->GetActorLocation());
	FVector MyDir = Me->GetActorLocation() - Target->GetActorLocation();
	Me->AddMovementInput(MyDir.GetSafeNormal());
	if (Target && MyDir.Size() < EscapeRange)
	{
		LittleSisterState = 1;
	}
	else if(Target && MyDir.Size() > EscapeRange)
	{
		LittleSisterState = 0;
	}
}

void ULittleSisterFSM::TickGiveUp()
{
	Me->GetCharacterMovement()->MaxWalkSpeed = 0;
	auto temp = UKismetMathLibrary::FindLookAtRotation(Me->GetActorLocation(), Target->GetActorLocation());
	Me->SetActorRelativeRotation(temp);
}


