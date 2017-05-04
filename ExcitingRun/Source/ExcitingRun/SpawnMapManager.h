// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnMapManager.generated.h"

UCLASS()
class EXCITINGRUN_API ASpawnMapManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnMapManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere) TArray<class ATargetPoint*> SpawnPoints;
	UPROPERTY(EditAnywhere) TSubclassOf<ACharacter> CharacterClass;
	UPROPERTY(EditAnywhere) float SpawnTime = 5.0f;

	void OnSpawnTimer();
	
};
