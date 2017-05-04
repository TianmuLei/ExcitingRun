// Fill out your copyright notice in the Description page of Project Settings.

#include "ExcitingRun.h"
#include "Weapon.h"
#include "Obstacle.h"
#include "Enemy.h"
#include "Sound/SoundCue.h"
// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
    PlayWeaponSound(BGM);
     //GetWorldTimerManager().SetTimer(WeaponTimer,this,&AWeapon::WeaponTrace,0.1f,true);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
   }

void AWeapon::WeaponTrace(){
    static FName WeaponFireTag = FName(TEXT("WeaponTrace"));
    static FName MuzzleSocket = FName(TEXT("MuzzleFlashSocket"));
    FVector StartPos = WeaponMesh->GetSocketLocation(MuzzleSocket);
    // Get forward vector of MyPawn
    FVector Forward =GetOwner()->GetActorForwardVector();
    // Calculate end position
    FVector EndPos = StartPos+Forward*WeaponRange;
    // Perform trace to retrieve hit info
    FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
    TraceParams.bTraceAsyncScene = true;
    TraceParams.bReturnPhysicalMaterial = true;
    // This fires the ray and checks against all objects w/ collision
    FHitResult Hit(ForceInit);
    GetWorld()->LineTraceSingleByObjectType(Hit, StartPos, EndPos,
                                            FCollisionObjectQueryParams::AllObjects, TraceParams);
    // Did this hit anything?
    if (Hit.bBlockingHit)
    {
        //UGameplayStatics::SpawnEmitterAtLocation(this, hitEffect, Hit.ImpactPoint);
        AObstacle* ob = Cast<AObstacle>(Hit.GetActor());
        if(ob){
            ob->Destroy();
        }
        AEnemy* en =Cast<AEnemy>(Hit.GetActor());
        if(en){
            en->Destroy();
            PlayWeaponSound(DeathSound);

        }
        
    }
}
void AWeapon::OnStartFire(){
    GetWorldTimerManager().SetTimer(WeaponTimer, this, &AWeapon::WeaponTrace, 0.1f,true);
    FireAC=PlayWeaponSound(FireLoopSound);
}
void AWeapon::OnStopFire(){
    GetWorldTimerManager().ClearTimer(WeaponTimer);
    if(FireAC){
        FireAC->Stop();
    }
}

UAudioComponent* AWeapon::PlayWeaponSound(USoundCue* Sound){
    UAudioComponent* AC=NULL;
    if(Sound){
        AC=UGameplayStatics::SpawnSoundAttached(Sound, RootComponent);
    }
    return AC;
}
