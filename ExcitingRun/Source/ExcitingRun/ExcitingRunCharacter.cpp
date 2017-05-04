// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ExcitingRun.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "ExcitingRunCharacter.h"
#include "FollowingCharacter.h"
#include "Engine/Blueprint.h"
#include "BP_FloorTile.h"
#include "Weapon.h"
#include <iostream>
#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "ExcitingRunGameMode.h"



FVector floorPosition;
int count = 1;
FVector floorDirection = FVector(1, 0, 0);

//////////////////////////////////////////////////////////////////////////
// AExcitingRunCharacter

AExcitingRunCharacter::AExcitingRunCharacter()
{
    showMoveHUD = false;
    MyWeapon = nullptr;
	floorDirection = FVector(1, 0, 0);//Start in the positive x direction
	floorPosition = FVector(1000, 0, -200);//initial floor position
	GetCharacterMovement()->MaxWalkSpeed = 800.0F;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->RelativeLocation =FVector(0.0f, 0.0f, 54.0f);
    SpringArm->SetupAttachment(RootComponent);
    SpringArm ->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
    SpringArm->TargetArmLength = 525.0f;
    SpringArm->bEnableCameraLag = false;
    SpringArm->bEnableCameraRotationLag = false;
    SpringArm->bInheritPitch = true;
    SpringArm->bInheritYaw = true;
    SpringArm->bInheritRoll = true;
    
	// Create the chase camera component 
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm,USpringArmComponent::SocketName);
    Camera->RelativeRotation =FRotator(10.0f, 0.0f, 0.0f);
    Camera->bUsePawnControlRotation = false;
    Camera->FieldOfView = 100.f;
    dead = false;

	direction = 0;//direction player is facing (values 0 to 3)
	shiftDirection = 0;
	totalDistance = 0;
	
    frameCount = 0;//use to count number of frames since last floor piece was created - see Tick() function below

    canTurnRight = true;//used to prevent the player turning more than 90 degrees on a single key press
	canShiftRight = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++1
	
	static ConstructorHelpers::FObjectFinder<UBlueprint> ASD1(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/MyBP_FloorTile'"));
	if (ASD1.Object != NULL) {
		BlueprintPath= (UClass*)ASD1.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> ASD2(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/BP_FloorTile_Corner'"));
	if (ASD1.Object != NULL) {
		BlueprintCorner = (UClass*)ASD2.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> ASD3(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/BP_FloorTile_Path_h'"));
	if (ASD3.Object != NULL) {
		BlueprintPath_h = (UClass*)ASD3.Object->GeneratedClass;
	}


}

void AExcitingRunCharacter::BeginPlay(){
    // Call base class BeginPlay
    Super::BeginPlay();
    // Spawn the weapon, if one was specified
    if (WeaponClass){
        UWorld* World = GetWorld();
        if(World){
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = Instigator;
            // Need to set rotation like this because otherwise gun points down
            FRotator Rotation(0.0f, 90.0f, 0.0f);
            MyWeapon = World->SpawnActor<AWeapon>(WeaponClass, FVector(20.0f,5.0f,70.0f),
                                                  Rotation, SpawnParams);
            if(MyWeapon){
                // This is attached to "WeaponPoint" which is defined in the skeleton
                MyWeapon->AttachToComponent(GetMesh(),
                                            FAttachmentTransformRules::KeepRelativeTransform,
                                            TEXT("WeaponPoint"));
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////////
// Input

void AExcitingRunCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    InputComponent->BindAction("Fire", IE_Pressed, this, &AExcitingRunCharacter::OnStartFire);
    InputComponent->BindAction("Fire", IE_Released, this, &AExcitingRunCharacter::OnStopFire);

	//PlayerInputComponent->BindAxis("MoveForward", this, &AExcitingRunCharacter::MoveForward);
	//InputComponent->BindAxis("MoveForward", this, &AJungleRunCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AExcitingRunCharacter::ChangeDirection);
	InputComponent->BindAxis("ShiftLeft", this, &AExcitingRunCharacter::ShiftRight);





	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
//	PlayerInputComponent->BindAxis("TurnRate", this, &AExcitingRunCharacter::TurnAtRate);
//	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
//	PlayerInputComponent->BindAxis("LookUpRate", this, &AExcitingRunCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AExcitingRunCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AExcitingRunCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AExcitingRunCharacter::OnResetVR);
}


void AExcitingRunCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AExcitingRunCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AExcitingRunCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AExcitingRunCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AExcitingRunCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AExcitingRunCharacter::MoveForward(float Value)
{
	
		// find out which way is forward
		//const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, 0, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	
}

void AExcitingRunCharacter::ShiftRight(float Value) {

	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, "shifttikcsomethin222222222222222g");
	if (Value != 0) {
		//canShiftRight = false;
		shiftDirection += Value;
		if (shiftDirection > 3)
			shiftDirection = 0;
		else if (shiftDirection < 0)
			shiftDirection = 3;
	}
	else if (Value == 0)
	{
		//canShiftRight = true;
	}


	
}

void AExcitingRunCharacter::shiftdirection(float Value) {


	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, "something");
	if ((Controller != NULL) && (Value != 0.0f))
	{
	
		if (direction == Forward)
		{
			if (Value > 0)
			{
				FVector newlocation = FVector(GetActorLocation().X, GetActorLocation().Y - 10, GetActorLocation().Z);
				SetActorLocation(newlocation);
			}
			else
			{
				FVector newlocation = FVector(GetActorLocation().X, GetActorLocation().Y + 10, GetActorLocation().Z);
				SetActorLocation(newlocation);

			}

		}
		else if (direction == Backward)
		{
			if (Value > 0)
			{
				FVector newlocation = FVector(GetActorLocation().X, GetActorLocation().Y + 10, GetActorLocation().Z);
				SetActorLocation(newlocation);
			}
			else
			{
				FVector newlocation = FVector(GetActorLocation().X, GetActorLocation().Y - 10, GetActorLocation().Z);
				SetActorLocation(newlocation);

			}
		}
		else if (direction == Right)
		{
			if (Value > 0)
			{
				FVector newlocation = FVector(GetActorLocation().X + 10, GetActorLocation().Y, GetActorLocation().Z);
				SetActorLocation(newlocation);
			}
			else
			{
				FVector newlocation = FVector(GetActorLocation().X - 10, GetActorLocation().Y, GetActorLocation().Z);
				SetActorLocation(newlocation);

			}
		}
			
		else if (direction == Left) {
			if (Value > 0)
			{
				FVector newlocation = FVector(GetActorLocation().X - 10, GetActorLocation().Y, GetActorLocation().Z);
				SetActorLocation(newlocation);
			}
			else
			{
				FVector newlocation = FVector(GetActorLocation().X + 10, GetActorLocation().Y, GetActorLocation().Z);
				SetActorLocation(newlocation);

			}
		}
		

		

	}
}


void AExcitingRunCharacter::MoveRight(float Value)
		// find out which way is right
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
		
	}
}

void AExcitingRunCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
//	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, totalDistance);
    if(bear){//spawn the bear
        FVector location =FVector(GetActorLocation().X-800,GetActorLocation().Y,GetActorLocation().Z);
        AFollowingCharacter* follow = GetWorld()->SpawnActor<AFollowingCharacter>(FollowingCharacterClass,location, FRotator(0,0,0));
        follow->SpawnDefaultController();
        bear=false;
    }
	if (direction == Forward)
		MoveForward(speed);
	else if (direction == Backward)
		MoveForward(-1*speed);
	else if (direction == Right)
		MoveRight(1*speed);
	else if (direction == Left)
		MoveRight(-1*speed);
    

	if (shiftDirection == Forward)
	{
		
	}
	else if (shiftDirection == Right) {
		shiftdirection(-1 * speed);
		shiftDirection = Forward;
	//	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, "shifttikcsomething");


	}
	else if (shiftDirection == Left) {
		shiftdirection(1 * speed);
		shiftDirection = Forward;


	}

	AExcitingRunGameMode* Mymode = Cast<AExcitingRunGameMode>(UGameplayStatics::GetGameMode(this));

	if (Mymode->getCollection() <= 0) {
		dead = true;
	}

    if(dead && firstdead==0){
		firstdead = 1;
        float duration = PlayAnimMontage(DieAnim);
        float actualDuration = duration - 0.25f;
        GetWorldTimerManager().SetTimer(timer, this, &AExcitingRunCharacter::Slay, actualDuration);
        APlayerController* PC = Cast<APlayerController>(GetController());
        if(PC){
            PC->SetCinematicMode(true, true, true);
        }
    }
    else if (dead){
        APlayerController* PC = Cast<APlayerController>(GetController());
        if(PC){
            PC->SetCinematicMode(true, true, true);
        }
        //GetMesh()->Deactivate();
    
    }
	else {
		totalDistance++;
	}
    
    
	ABP_FloorTile *myFloor;

	UWorld* const World = GetWorld();
	if (World && frameCount++ > 30) {
		frameCount = 0;
		FVector SpawnLocation = floorPosition;
        if(NumofFloor==30){
            return;
        }
        NumofFloor++;
		floorPosition += floorDirection * 1000;
		FRotator FloorRotation = FRotator(0, 0, 0);
		if (floorDirection != floorDirectionOld) {
			/*if (floorDirection == FVector(0, -1, 0)) {
				myFloor = World->SpawnActor<ABP_FloorTile>(BlueprintCorner, FVector(SpawnLocation.X+500, SpawnLocation.Y-500, SpawnLocation.Z), FRotator(0, 90, 0));
				floorDirectionOld = floorDirection;
			}
			else if (floorDirection == FVector(0, 1, 0)) {
				myFloor = World->SpawnActor<ABP_FloorTile>(BlueprintCorner, SpawnLocation, FloorRotation);
				floorDirectionOld = floorDirection;
			}
			else if (floorDirection == FVector(1, 0, 0)) {
				myFloor = World->SpawnActor<ABP_FloorTile>(BlueprintCorner, FVector(SpawnLocation.X, SpawnLocation.Y , SpawnLocation.Z), FRotator(0, -90, 0));
				floorDirectionOld = floorDirection;

			}
			else {
				myFloor = World->SpawnActor<ABP_FloorTile>(BlueprintCorner, FVector(SpawnLocation.X+500, SpawnLocation.Y-500, SpawnLocation.Z), FRotator(0, 90, 0));
				floorDirectionOld = floorDirection;
			}*/
			myFloor = World->SpawnActor<ABP_FloorTile>(BlueprintCorner, SpawnLocation, FloorRotation);
            myFloor->showTutorial = showTutorial;
            myFloor->JumpTutorial = JumpTutorial;
			floorDirectionOld = floorDirection;
				
		}
		else {
            if (floorDirection == FVector(0,1,0) || floorDirection == FVector(0,-1,0)){
                myFloor = World->SpawnActor<ABP_FloorTile>(BlueprintPath_h, SpawnLocation, FloorRotation);
                myFloor->showTutorial = showTutorial;
                myFloor->JumpTutorial = JumpTutorial;
                if(floorDirection==FVector(0,1,0)){
                    myFloor->direction=Right;
                }
                else {
                    myFloor->direction=Left;
                }
            }
			else {
				myFloor = World->SpawnActor<ABP_FloorTile>(BlueprintPath, SpawnLocation, FloorRotation);
                myFloor->showTutorial = showTutorial;
                myFloor->JumpTutorial = JumpTutorial;
                if(floorDirection==FVector(1,0,0)){
                    myFloor->direction=Forward;
                }
                else {
                    myFloor->direction=Backward;
                }
			}
		}
        myFloor->createObstacle();
		myFloor->createLowerObstacle();
		updateFloorDirection(floorDirection);
        
	}
	if (World && init_state++ < 5) {
		FVector SpawnLocation = floorPosition;
		floorPosition += floorDirection * 1000;//size of the floor is 1000

		const FRotator FloorRotation = FRotator(0, 0, 0);
		myFloor = World->SpawnActor<ABP_FloorTile>(BlueprintPath, SpawnLocation, FloorRotation);
		floorDirectionOld = floorDirection;
        if(floorDirection==FVector(1,0,0)){
            myFloor->direction=Forward;
        }
        else if(floorDirection==FVector(-1,0,0)){
            myFloor->direction=Backward;
        }
        else if(floorDirection==FVector(0,1,0)){
            myFloor->direction=Right;
        }
        else if(floorDirection==FVector(0,-1,0)){
            myFloor->direction=Left;
        }
	}
    float dist = FVector::Dist(GetActorLocation(),FirstOb);
    float dist2 = FVector::Dist(GetActorLocation(), FirstLowerOb);
    //std::cout << "Distance2: " << dist2 << std::endl;
    if(dist < 2000.0f){
        showMoveHUD = true;
    }
    else{
        showMoveHUD = false;
    }
    
    if(dist2 < 2000.0f){
        showJumpHUD = true;
    }
    else{
        showJumpHUD = false;
    }
}

void AExcitingRunCharacter::Slay(){
    GetMesh()->Deactivate();
}

void AExcitingRunCharacter::ChangeDirection(float Value)
{
    
//    if (Value && canTurnRight) {
//        canTurnRight = false;
//        int temp=direction;
//        int orginalDirection=temp;
//        temp+=Value;
//        if (temp > 3){
//            temp = 0;
//        }
//        else if (temp < 0){
//           temp = 3;
//        }
//        if(forbidenDirection==temp){
//            temp=orginalDirection;
//        }
//        direction=temp;
//    }
//    else if (Value == 0){
//        canTurnRight = true;
//    }
    
    
    if (Value != 0 && canTurnRight) {
        canTurnRight = false;
        direction += Value;
        if(showTurnHUD){
            showTurnHUD=false;
            std::cout << "IN TURN" << std::endl;
        }
        if (direction > 3)
            direction = 0;
        else if (direction < 0)
            direction = 3;
    }
    else if (Value == 0)
        canTurnRight = true;
}


void AExcitingRunCharacter::updateFloorDirection(FVector& direction) {
    int rand =FMath::RandRange(0,12);
    if(rand ==0&&floorDirection.X!=-1){
        floorDirection=FVector(1,0,0);
    }
    else if(rand==1&&floorDirection.X!=1){
        floorDirection=FVector(-1,0,0);
    }
    else if(rand ==2&&floorDirection.Y!=-1){
        floorDirection=FVector(0,1,0);
    }
    else if(rand==3&&floorDirection.Y!=1){
        floorDirection=FVector(0,-1,0);
    }

}

void AExcitingRunCharacter::OnStartFire(){
    if(MyWeapon){
        MyWeapon->OnStartFire();
    }
}
void AExcitingRunCharacter::OnStopFire(){
    if(MyWeapon){
        MyWeapon->OnStopFire();
    }
}

void AExcitingRunCharacter::setShowTutorial(){
    showTutorial = true;
}

void AExcitingRunCharacter::setJumpTutorial(){
    JumpTutorial = true;
}
