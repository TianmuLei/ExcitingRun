// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "ExcitingRunGameMode.generated.h"

UCLASS(minimalapi)
class AExcitingRunGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AExcitingRunGameMode();
	void AddCollection();
	void reduceCollection();
	int collection=1;
	int getCollection() { return collection; }
};



