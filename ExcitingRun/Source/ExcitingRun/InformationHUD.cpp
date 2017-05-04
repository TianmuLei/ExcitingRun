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
	//FVector2D ScreenDimensions = FVector2D(800,600);
	Super::DrawHUD();
	AExcitingRunCharacter* MyCharater = Cast<AExcitingRunCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

    if(MyCharater != nullptr){
        FString IntAsString = FString::FromInt(MyCharater->totalDistance);
        FString PowerLevelString = "Score: "+IntAsString;
        DrawText(PowerLevelString, FColor::White, 50, 50, HUDFont);
    }

	//FString PowerLevelString = "Score: "+IntAsString;
	//DrawText(PowerLevelString, FColor::White, 50, 50, HUDFont);


	AExcitingRunGameMode* Mymode = Cast<AExcitingRunGameMode>(UGameplayStatics::GetGameMode(this));
	int number= Mymode->getCollection();

	FString IntAsStringCollection = FString::FromInt(number);

	FString CollectionString = "Collection: "+ IntAsStringCollection;
	DrawText(CollectionString, FColor::White, 50, 80, HUDFont);
    
    if(MyCharater != nullptr){
        if(MyCharater->showMoveHUD){
            MoveTutorial = "Press Q or E to move Left or Right";
            DrawText(MoveTutorial, FColor::Blue, 50, 100, HUDFont);
        }
        if(MyCharater->showJumpHUD){
            JumpTutorial = "Press Space to Jump";
            DrawText(JumpTutorial, FColor::Yellow, 50, 120, HUDFont);
        }
        if (MyCharater->dead) {
            FString overString = "GAME OVER";
            DrawText(overString, FColor::Red, 550, 350, HUDFont);
        }
        if(MyCharater->showTurnHUD){
            TurnTutorial = "Press A to turn Left and D to turn Right";
            DrawText(TurnTutorial, FColor::Green, 50, 200, HUDFont);
            std::cout << "IN TURN HUD" << std::endl;
        }
    }
	



}

void AInformationHUD::AddCollection() {
	collection = collection + 1;
}
