// Fill out your copyright notice in the Description page of Project Settings.

#include "ExcitingRun.h"
#include "FollowingCharacter.h"
#include "FollowingAIController.h"


// Sets default values
AFollowingCharacter::AFollowingCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AFollowingAIController::StaticClass();

}

// Called when the game starts or when spawned
void AFollowingCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = 800.0F;
    
	
}

// Called every frame
void AFollowingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFollowingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//void AFollowingCharacter::BeginTouch(AExcitingRunCharacter character){
    
//}
void AFollowingCharacter::speedup(){
    FTimerHandle timer;
    GetWorldTimerManager().SetTimer(timer, this, &AFollowingCharacter::slowdown,3.0);
    GetCharacterMovement()->MaxWalkSpeed = 1200.0F;
}

void AFollowingCharacter::slowdown(){
    GetCharacterMovement()->MaxWalkSpeed = 800.0F;
}
