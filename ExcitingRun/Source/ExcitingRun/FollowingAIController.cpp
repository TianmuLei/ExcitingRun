// Fill out your copyright notice in the Description page of Project Settings.

#include "ExcitingRun.h"
#include "FollowingAIController.h"
#include "ExcitingRunCharacter.h"
#include "FollowingCharacter.h"
#include <iostream>


void AFollowingAIController::BeginPlay() {
	Super::BeginPlay();
    player = UGameplayStatics::GetPlayerPawn(this, 0);
	mStatus = Start;
    range = 140.0f;
}

void AFollowingAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	player = UGameplayStatics::GetPlayerPawn(this, 0);
	MoveToActor(player);
    AFollowingCharacter* FollowPawn = Cast<AFollowingCharacter>(GetPawn());
    AExcitingRunCharacter* PlayerPawn = Cast<AExcitingRunCharacter>(player);
    if(FollowPawn == nullptr){
        //std::cout << "FollowPawn null" << std::endl;
    }
    if(PlayerPawn == nullptr){
        //std::cout << "playerpawn null" << std::endl;
    }
    if(FollowPawn && PlayerPawn){
        //std::cout << "IN if" << std::endl;
        float Dist = FVector::Dist(player->GetActorLocation(), GetPawn()->GetActorLocation());
        //std::cout << Dist << std::endl;
        //std::cout << range << std::endl;
        if(Dist < range){
            //std::cout << "IN dist" << std::endl;
            PlayerPawn->dead = true;
        }
    }
    //std::cout << "DEAD: " << PlayerPawn->dead << std::endl;
    /*if(mStatus == Start){
        MoveToActor(player);
        mStatus = Chase;
    }
    else if(mStatus == Chase){
        AFollowingCharacter* FollowPawn = Cast<AFollowingCharacter>(GetPawn());
        AExcitingRunCharacter* PlayerPawn = Cast<AExcitingRunCharacter>(player);
        if(FollowPawn && PlayerPawn){
            float Dist = FVector::Dist(player->GetActorLocation(), GetPawn()->GetActorLocation());
            if(Dist < range){
                PlayerPawn->dead = true;
            }
        }
    }*/
};




