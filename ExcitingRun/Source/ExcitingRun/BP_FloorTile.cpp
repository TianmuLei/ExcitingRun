// Fill out your copyright notice in the Description page of Project Settings.

#include "ExcitingRun.h"
#include "BP_FloorTile.h"
#include <iostream>
#include "Obstacle.h"
#include "Collectable.h"
#include "LowerObstacle.h"
#include "ExcitingRunCharacter.h"
#include "CollectIncible.h"
#include "Enemy.h"
#include <iostream>
#include "Ammo.h"

using namespace std;

// Sets default values
ABP_FloorTile::ABP_FloorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UBlueprint> ASD1(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/MyLowerObstacleBP'"));
	if (ASD1.Object != NULL) {
		LowerObstacleClass = (UClass*)ASD1.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> ASD2(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/MyCollectIncibleBP'"));
	if (ASD2.Object != NULL) {
		CollectIncibleClass = (UClass*)ASD2.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void ABP_FloorTile::BeginPlay()
{
	Super::BeginPlay();
    pawn=UGameplayStatics::GetPlayerPawn(this, 0);
    int rand =FMath::RandRange(0,4);
    int rand2 =FMath::RandRange(0, 9);
    int rand22 =(FMath::RandRange(0, 9)+5)%10;
    int rand23 =(FMath::RandRange(0, 9)+3)%10;

    int rand3 =FMath::RandRange(0, 15);
    int rand4=FMath::RandRange(0, 6);
    FVector temp = FVector(GetActorLocation().X+100*rand2,GetActorLocation().Y+20*rand,GetActorLocation().Z);
    ACollectable* collect =GetWorld()->SpawnActor<ACollectable>(CollectableClass,temp,FRotator(0,0,0));
	
    if (rand3 == 2) {
		temp = FVector(GetActorLocation().X + 100 * rand2, GetActorLocation().Y + 50 * rand, GetActorLocation().Z);
		ACollectIncible* collectIn = GetWorld()->SpawnActor<ACollectIncible>(CollectIncibleClass, temp, FRotator(0, 0, 0));
		collectInv.Add(collectIn);

	}
    collectables.Add(collect);

	
    if(rand==0||rand==1){
        FVector temp11 = FVector(GetActorLocation().X+100*rand22,GetActorLocation().Y+50*rand23,GetActorLocation().Z);
        GetWorld()->SpawnActor<AAmmo>(AmmoClass, temp11, FRotator(0, 0, 0));
   }
	
	if(rand2==1){
        AEnemy* enemy =GetWorld()->SpawnActor<AEnemy>(EnemyClass, temp, FRotator(0,0,0));
		if (enemy) {
			enemy->SpawnDefaultController();
			Cast<AExcitingRunCharacter>(pawn)->Enemies.Add(enemy);
		}
    }
}

// Called every frame
void ABP_FloorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if(isInside()&&!willDelete){
        willDelete=true;
    }
    if(willDelete&&!isInside()){
        FTimerHandle timer;
        GetWorldTimerManager().SetTimer(timer, this, &ABP_FloorTile::dis, 4.0f, false);
    }
}

void ABP_FloorTile::dis() {
    Cast<AExcitingRunCharacter>(pawn)->NumofFloor--;
    Destroy();
    for(int i=0;i<obstacles.Num();++i){
        obstacles[i]->Destroy();
    }
    for(int i=0;i<collectables.Num();++i){
        collectables[i]->Destroy();
    }
	for (int i = 0; i<Lowerobstacles.Num(); ++i) {
		Lowerobstacles[i]->Destroy();
	}
	for (int i = 0; i<collectInv.Num(); ++i) {
		collectInv[i]->Destroy();
	}


}
bool ABP_FloorTile::isInside(){
    //load the forbiden Direction of the direction
    FVector actorLocation =pawn->GetActorLocation();

    //see if the character is inside the tile
    FVector origin = GetActorLocation();
    FVector corner1= FVector(origin.X,origin.Y+500,origin.Z);
    FVector corner2=FVector(origin.X,origin.Y-500,origin.Z);
    FVector corner4=FVector(origin.X+1000,origin.Y+500,origin.Z);
    FVector corner3=FVector(origin.X+1000,origin.Y-500,origin.Z);
    if(actorLocation.X>corner1.X&&actorLocation.X<corner4.X&&
       actorLocation.Y>corner2.Y&&actorLocation.Y<corner4.Y){
        setForbidden();
        return true;
    }
    if(actorLocation.X==corner1.X&&actorLocation.X==corner4.X&&
       actorLocation.Y==corner2.Y&&actorLocation.Y==corner4.Y){
        setForbidden();
        return true;
    }
    return false;
}

void ABP_FloorTile::createObstacle(){
    int rand =FMath::RandRange(0,4);
    int rand2 =FMath::RandRange(0, 9);
    if(rand!=1){
        return;
    }
    FVector temp = FVector(GetActorLocation().X+100*rand2,GetActorLocation().Y+20*rand,GetActorLocation().Z);
    if(!showTutorial){
        Cast<AExcitingRunCharacter>(pawn)->FirstOb = temp;
        showTutorial = true;
        Cast<AExcitingRunCharacter>(pawn)->setShowTutorial();
    }
    FRotator rotate =FRotator(0,0,0);
    if(direction ==Right||direction==Left){
        rotate =FRotator(0,-90.0f,0);
    }
    AObstacle* myObstacle = GetWorld()->SpawnActor<AObstacle>(ObstacleClass,temp,rotate);
    obstacles.Add(myObstacle);
}

void ABP_FloorTile::createLowerObstacle() {
	int rand = FMath::RandRange(0, 2);
	int rand2 = FMath::RandRange(3, 7);
	if (rand != 1) {
		return;
	}
	FVector temp = FVector(GetActorLocation().X + 100 * rand2, GetActorLocation().Y+ 20* rand2, GetActorLocation().Z);
    if(!JumpTutorial){
        Cast<AExcitingRunCharacter>(pawn)->FirstLowerOb = temp;
        float haha = FVector::Dist(GetActorLocation(), temp);
        //cout << "First Calcted: " << haha << endl;
        JumpTutorial = true;
        Cast<AExcitingRunCharacter>(pawn)->setJumpTutorial();
    }
	FRotator rotate = FRotator(0, 0, 0);
	if (direction == Right || direction == Left) {
		rotate = FRotator(0, -90.0f, 0);
	}
	ALowerObstacle* myObstacle = GetWorld()->SpawnActor<ALowerObstacle>(LowerObstacleClass, temp, rotate);
	Lowerobstacles.Add(myObstacle);
}

void ABP_FloorTile::setForbidden(){
    if(direction==Forward){
        Cast<AExcitingRunCharacter>(pawn)->forbidenDirection=Forward;
    }
    else if(direction==Backward){
        Cast<AExcitingRunCharacter>(pawn)->forbidenDirection=Backward;
    }
    else if(direction==Right){
        Cast<AExcitingRunCharacter>(pawn)->forbidenDirection=Left;
    }
    else if (direction==Left){
        Cast<AExcitingRunCharacter>(pawn)->forbidenDirection=Right;
    }
    //Cast<AExcitingRunCharacter>(pawn)->forbidenDirection=direction;
}
