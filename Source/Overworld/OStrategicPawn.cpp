// Fill out your copyright notice in the Description page of Project Settings.


#include "OStrategicPawn.h"


#include "NavigationSystem.h"
#include "OInteractive.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

class UNavigationSystemV1;
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
	APlayerController* pc = Cast<APlayerController>(Controller);
	if(pc)
	{
		pc->bShowMouseCursor = true;
		pc->DefaultMouseCursor = EMouseCursor::Crosshairs;
	}

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AOStrategicPawn::OnSelectObject);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AOStrategicPawn::OnSelectSecondary);
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
		AddMovementInput(FVector::ForwardVector, Value);
	}
}

void AOStrategicPawn::OnMoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(FVector::RightVector, Value);
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

		if (Hit.bBlockingHit && Hit.GetActor()->GetClass()->ImplementsInterface(UOInteractive::StaticClass()))
		{
			IOInteractive::Execute_OnInteractStart(Hit.GetActor());
			SelectedActor = Hit.GetActor();
		} else if (SelectedActor != nullptr)
		{
			IOInteractive::Execute_OnInteractStop(SelectedActor);
			SelectedActor = nullptr;
		}
	}
}

void AOStrategicPawn::OnSelectSecondary()
{
	APlayerController* pc = Cast<APlayerController>(Controller);

	if (pc && SelectedActor != nullptr)
	{
		// Trace to see what is under the touch location
		FHitResult Hit;
		pc->GetHitResultUnderCursor(pc->CurrentClickTraceChannel, true, Hit);

		if (Hit.bBlockingHit && Hit.GetActor()->GetClass()->ImplementsInterface(UOInteractive::StaticClass()))
		{
			IOInteractive::Execute_OnTandemInteractActor(SelectedActor, Hit.GetActor());
		} else
		{
			FVector NavMeshLocation = GetPointOnNavMesh(Hit.ImpactPoint);

			if (NavMeshLocation != FVector::ZeroVector)
			{
				IOInteractive::Execute_OnTandemInteractLocation(SelectedActor, NavMeshLocation);
			}
		}
	}
}

FVector AOStrategicPawn::GetPointOnNavMesh(FVector point) const
{
	FNavLocation location;
	const FNavAgentProperties* navAgentProperties = &GetNavAgentPropertiesRef();
	UNavigationSystemV1* navSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());

	if (navSystem == nullptr)
	{
		return point;
	}

	navSystem->ProjectPointToNavigation(point, location, FVector(5, 5, 2000), navAgentProperties);
	return location.Location;
}