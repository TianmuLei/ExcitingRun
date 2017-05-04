// Fill out your copyright notice in the Description page of Project Settings.

#include "ExcitingRun.h"
#include "Ammo.h"

#include "ExcitingRunGameMode.h"
// Sets default values
AAmmo::AAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    
    APawn* pawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if(pawn==nullptr){

        return;
    }
    FVector actorLocation = pawn->GetActorLocation();
    FVector itemLocation = GetActorLocation();
    float distance = FVector::Dist(actorLocation, itemLocation);
    if (distance<150) {
        
        AExcitingRunGameMode* Mymode = Cast<AExcitingRunGameMode>(UGameplayStatics::GetGameMode(this));
        Mymode->ammo=Mymode->ammo+30;
        Destroy();
    }

}

