// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "FollowingAIController.generated.h"

/**
 * 
 */
UCLASS()
class EXCITINGRUN_API AFollowingAIController : public AAIController
{
	GENERATED_BODY()
	
	
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	APawn* player = nullptr;
    enum AIStatus{
        Start,
        Chase,
        Attack
        //Dead
    };
    AIStatus mStatus;
    float range;
};
