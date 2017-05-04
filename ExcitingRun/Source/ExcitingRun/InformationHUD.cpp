// Fill out your copyright notice in the Description page of Project Settings.

#include "ExcitingRun.h"
#include "InformationHUD.h"
#include "ExcitingRunGameMode.h"
#include "ExcitingRunCharacter.h"
#include "Engine/Font.h"
#include <iostream>




AInformationHUD::AInformationHUD(const class FObjectInitializer & PCIP): Super(PCIP) {

	static ConstructorHelpers::FObjectFinder<UFont>HUDFontOb(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	if (HUDFontOb.Object!=NULL)
	HUDFont = HUDFontOb.Object;


}

void AInformationHUD::DrawHUD() {
    Super::DrawHUD();
	AExcitingRunCharacter* MyCharater = Cast<AExcitingRunCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

    if(MyCharater != nullptr){
        FString IntAsString = FString::FromInt(MyCharater->totalDistance);
        FString PowerLevelString = "Score: "+IntAsString;
        DrawText(PowerLevelString, FColor::White, 50, 50, HUDFont);
    }

	AExcitingRunGameMode* Mymode = Cast<AExcitingRunGameMode>(UGameplayStatics::GetGameMode(this));
	int number= Mymode->getCollection();
	bool invincible = Mymode->getInvincible();

	FString IntAsStringCollection = FString::FromInt(number);

	FString CollectionString = "Collection: "+ IntAsStringCollection;
	DrawText(CollectionString, FColor::White, 50, 80, HUDFont);
    
    FString IntAsStringAmmo = FString::FromInt(Mymode->ammo);
    FString Ammo = "Ammo: " + IntAsStringAmmo;
    DrawText(Ammo, FColor::White, 50, 110, HUDFont);
    
    if(MyCharater != nullptr){
        if(MyCharater->showMoveHUD){
            MoveTutorial = "Press Q or E to move Left or Right";
            DrawText(MoveTutorial, FColor::Blue, 200, 70, HUDFont);
        }
        if(MyCharater->showJumpHUD){
            JumpTutorial = "Press Space to Jump";
            DrawText(JumpTutorial, FColor::Yellow, 200, 100, HUDFont);
        }
        if (MyCharater->dead) {
            FString overString = "GAME OVER";
            DrawText(overString, FColor::Red, 300, 300, HUDFont);
        }
        if(MyCharater->showTurnHUD){
            TurnTutorial = "Press A to turn Left and D to turn Right";
            DrawText(TurnTutorial, FColor::Green, 200, 130, HUDFont);
            std::cout << "IN TURN HUD" << std::endl;
        }
		if (invincible == true) {
			FString Invincible = "Invincible!";
			DrawText(Invincible, FColor::Red, 50, 230, HUDFont);
		}
        if(MyCharater->showShootHUD){
            ShootTutorial = "Click Left Button to Shoot!";
            DrawText(ShootTutorial, FColor::Red, 200, 160, HUDFont);
        }
    }
	



}

void AInformationHUD::AddCollection() {
	collection = collection + 1;
}
