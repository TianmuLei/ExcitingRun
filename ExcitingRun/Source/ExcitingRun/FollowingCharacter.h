// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "FollowingCharacter.generated.h"
//#include "ExcitingRunCharacter.h"

UCLASS()
class EXCITINGRUN_API AFollowingCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFollowingCharacter();
    void speedup();
    void slowdown();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    //virtual void BeginTouch(AExcitingRunCharacter character) override;
};
