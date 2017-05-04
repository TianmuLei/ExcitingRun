// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ExcitingRun.h"
#include "ExcitingRunGameMode.h"
#include "ExcitingRunCharacter.h"
#include "InformationHUD.h"

AExcitingRunGameMode::AExcitingRunGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	HUDClass = AInformationHUD::StaticClass();
}

void AExcitingRunGameMode::AddCollection() {
	collection = collection + 1;
}
void AExcitingRunGameMode::reduceCollection() {
	collection = collection - 5;

}


