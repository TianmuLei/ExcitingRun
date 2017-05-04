// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class EXCITINGRUN_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
protected:
    void BeginPlay() override;
    void Tick(float DeltaTime) override;
public:
    APawn* player = nullptr;
    enum AIStatus{
        Start,
        Chase,
        Attack,
        Dead
    };
    AIStatus mStatus;
};
