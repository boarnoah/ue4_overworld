// Fill out your copyright notice in the Description page of Project Settings.


#include "OStrategicPawn.h"

#include "OInteractive.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AOStrategicPawn::AOStrategicPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->SetUpdatedComponent(GetRootComponent());
}

// Called when the game starts or when spawned
void AOStrategicPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOStrategicPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AOStrategicPawn::OnSelectObject);
	InputComponent->BindAxis("MoveForward", this, &AOStrategicPawn::OnMoveForward);
	InputComponent->BindAxis("MoveRight", this, &AOStrategicPawn::OnMoveRight);
}

// Called every frame
void AOStrategicPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOStrategicPawn::OnMoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AOStrategicPawn::OnMoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AOStrategicPawn::OnSelectObject()
{
	APlayerController* pc = Cast<APlayerController>(Controller);

	if (pc)
	{
		// Trace to see what is under the touch location
		FHitResult Hit;
		pc->GetHitResultUnderCursor(pc->CurrentClickTraceChannel, true, Hit);

		if (Hit.bBlockingHit && Hit.GetComponent()->GetClass()->ImplementsInterface(UOInteractive::StaticClass()))
		{
			IOInteractive::Execute_OnInteractStart(Hit.GetComponent());
		}
	}
}

