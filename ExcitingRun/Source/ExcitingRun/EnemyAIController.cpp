// Fill out your copyright notice in the Description page of Project Settings.

#include "ExcitingRun.h"
#include "EnemyAIController.h"
#include "ExcitingRunCharacter.h"
#include "Enemy.h"


void AEnemyAIController::BeginPlay() {
    Super::BeginPlay();
    player = UGameplayStatics::GetPlayerPawn(this, 0);
    mStatus = Start;
}

void AEnemyAIController::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    player = UGameplayStatics::GetPlayerPawn(this, 0);
    MoveToActor(player);
    AEnemy* FollowPawn = Cast<AEnemy>(GetPawn());
    AExcitingRunCharacter* PlayerPawn = Cast<AExcitingRunCharacter>(player);
    if(FollowPawn && PlayerPawn){
        float Dist = FVector::Dist(player->GetActorLocation(), GetPawn()->GetActorLocation());
        if(Dist < 140.0f){
            //PlayerPawn->dead = true;
           // FollowPawn->Destroy();
        }
    }
}
