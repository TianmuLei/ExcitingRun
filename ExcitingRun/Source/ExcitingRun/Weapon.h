// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class EXCITINGRUN_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
   

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Weapon)
    USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditDefaultsOnly) UParticleSystem* HitEffect;
    
    UPROPERTY(EditDefaultsOnly, Category = Sound)
    class USoundCue* FireLoopSound;
    UPROPERTY(EditDefaultsOnly, Category = Sound)
    class USoundCue* FireFinishSound;
    UPROPERTY(Transient)
    class UAudioComponent* FireAC;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    void WeaponTrace();
    float WeaponRange =1000.0f;
    FTimerHandle WeaponTimer;
    void OnStartFire();
    void OnStopFire();
    UAudioComponent* PlayWeaponSound(USoundCue* Sound);
	
};
