// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BP_FloorTile.generated.h"
UCLASS()
class EXCITINGRUN_API ABP_FloorTile : public AActor
{
	GENERATED_BODY()
	APawn* pawn;
    void setForbidden();
public:	
	// Sets default values for this actor's properties
	ABP_FloorTile();
    int direction;
    enum Dir{Forward, Right, Backward, Left};
    bool willDelete=false;
    bool isInside();
    TArray <class AObstacle*> obstacles;
	TArray <class ALowerObstacle*> Lowerobstacles;
    TArray <class ACollectable*> collectables;
    bool showTutorial = false;
    bool JumpTutorial = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    UPROPERTY(EditAnywhere, Category=Obstacle)
		TSubclassOf<AActor> ObstacleClass;


		TSubclassOf<AActor> LowerObstacleClass;

    UPROPERTY(EditAnywhere, Category=Collectable)
    TSubclassOf<AActor> CollectableClass;
    UPROPERTY(EditAnywhere, Category=Enemy)
    TSubclassOf<ACharacter> EnemyClass;
    
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    void createObstacle();
	void createLowerObstacle();
	void dis();
    FVector FirstPos = FVector(0,0,0);
	
};
