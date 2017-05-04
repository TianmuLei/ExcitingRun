// Fill out your copyright notice in the Description page of Project Settings.

#include "ExcitingRun.h"
#include "Collectable.h"
#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "InformationHUD.h"
#include "ExcitingRunGameMode.h"

// Sets default values
ACollectable::ACollectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACollectable::BeginPlay()
{
	Super::BeginPlay();
	pawn=UGameplayStatics::GetPlayerPawn(this, 0);
}

// Called every frame
void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    FVector actorLocation =pawn->GetActorLocation();
    FVector itemLocation = GetActorLocation();
    float distance = FVector::Dist(actorLocation, itemLocation);
    if(distance<220){
		//GEngine->AddOnScreenDebugMessage(-1, 1.9f, FColor::Yellow, "Point +1");

		AExcitingRunGameMode* Mymode= Cast<AExcitingRunGameMode>(UGameplayStatics::GetGameMode(this));
		Mymode->AddCollection();
        Destroy();
    }

}

