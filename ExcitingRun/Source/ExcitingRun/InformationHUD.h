// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "InformationHUD.generated.h"

/**
 * 
 */
UCLASS()
class EXCITINGRUN_API AInformationHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

		UPROPERTY() UFont* HUDFont;

	//AInformationHUD(const FObjectInitializer & PCIP);

	
	virtual void DrawHUD() override;
	
	
public:
    FString MoveTutorial;
    FString JumpTutorial;
    FString TurnTutorial;
	int collection = 0;
	void AddCollection();
};
