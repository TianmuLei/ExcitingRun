// Fill out your copyright notice in the Description page of Project Settings.

#include "ExcitingRun.h"
#include "Enemy.h"
#include "EnemyAIController.h"
#include "ExcitingRunGameMode.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    AIControllerClass = AEnemyAIController::StaticClass();
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    
    AExcitingRunGameMode* Mymode = Cast<AExcitingRunGameMode>(UGameplayStatics::GetGameMode(this));
    
    if(Mymode->getInvincible()){
    
        APawn* pawn=UGameplayStatics::GetPlayerPawn(this, 0);
        FVector actorLocation =pawn->GetActorLocation();
        FVector itemLocation = GetActorLocation();
        float distance = FVector::Dist(actorLocation, itemLocation);
        
        
        if(distance<150.0f){
            Destroy();
            
        }
        
    }

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

