// Fill out your copyright notice in the Description page of Project Settings.

#include "ExcitingRun.h"
#include "Obstacle.h"
#include "ExcitingRunGameMode.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    AExcitingRunGameMode* Mymode = Cast<AExcitingRunGameMode>(UGameplayStatics::GetGameMode(this));

    if(Mymode->getInvincible()){
        pawn=UGameplayStatics::GetPlayerPawn(this, 0);
        FVector actorLocation =pawn->GetActorLocation();
        FVector itemLocation = GetActorLocation();
        float distance = FVector::Dist(actorLocation, itemLocation);
        if(distance<400.0f){
            Destroy();
        }
            
    }
    
    
        
}

