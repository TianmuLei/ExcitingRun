// Fill out your copyright notice in the Description page of Project Settings.

#include "ExcitingRun.h"
#include "SpawnMapManager.h"
#include "Engine/TargetPoint.h"



// Sets default values
ASpawnMapManager::ASpawnMapManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnMapManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnMapManager::Tick(float DeltaTime)
{
	Super::BeginPlay();
	FTimerHandle timer;
	GetWorldTimerManager().SetTimer(timer, this, &ASpawnMapManager::OnSpawnTimer, SpawnTime, true);


}

void ASpawnMapManager::OnSpawnTimer() {
	if (SpawnPoints.Num() == 0) {
		return;
	}

	int random = FMath::RandRange(0, SpawnPoints.Num() - 1);
	FVector pos = SpawnPoints[random]->GetActorLocation();
	FRotator rot = SpawnPoints[random]->GetActorRotation();
	ACharacter* Char = GetWorld()->SpawnActor<ACharacter>(CharacterClass, pos, rot);
	if (Char) {
		Char->SpawnDefaultController();
	}

}

