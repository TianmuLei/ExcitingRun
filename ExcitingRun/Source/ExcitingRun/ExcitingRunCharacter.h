// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "ExcitingRunCharacter.generated.h"


UCLASS(config=Game)
class AExcitingRunCharacter : public ACharacter
{
	GENERATED_BODY()
		/** Spring arm that will offset the camera */
		UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Camera;
    
    
	int direction;
    enum Dir{Forward, Right, Backward, Left};
    float speed =1.0f;

	FVector floorPosition;
	int frameCount;
	int init_state = 0;
	FVector floorDirection;
	FVector floorDirectionOld;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
    //class ABP_FloorTile* myFloor;
    bool showMoveHUD;
    bool showJumpHUD;
    bool showTurnHUD = true;
	int firstdead = 0;
    FVector FirstOb = FVector(0,0,0);
    FVector FirstLowerOb = FVector(0,0,0);
    bool showTutorial = false;
    bool JumpTutorial = false;
	AExcitingRunCharacter();
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

    bool dead;
    UPROPERTY(EditDefaultsOnly)
    UAnimMontage* DieAnim;
    FTimerHandle timer;
    void Slay();
    int forbidenDirection=Backward;
    bool canTurnRight;
	bool canShiftRight;
    int NumofFloor=5;
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);
	void ChangeDirection(float Value);

	virtual void Tick(float DeltaSeconds);
    
    void setShowTutorial();
    void setJumpTutorial();

	void updateFloorDirection(FVector& direction);
	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    UPROPERTY(EditAnywhere, Category=FollowingCharacter)
    TSubclassOf<ACharacter> FollowingCharacterClass;
    UPROPERTY(EditAnywhere, Category = Weapon)
    TSubclassOf<class AWeapon> WeaponClass;

public:
	TSubclassOf<AActor> BlueprintPath;
	TSubclassOf<AActor> BlueprintCorner;
	TSubclassOf<AActor> BlueprintPath_h;
    TArray<class AEnemy*> Enemies;
    bool bear =true;
	bool signtochange = false;
    void BeginPlay() override;
    void OnStartFire();
    void OnStopFire();
	void ShiftLeft(float Value);
	void ShiftRight(float Value);
	int shiftDirection = 0;
	void shiftdirection(float Value);
	int totalDistance = 0;
    void SlowDown();


private:
    class AWeapon* MyWeapon;
   


};

