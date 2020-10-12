// Fill out your copyright notice in the Description page of Project Settings.


#include "OStrategicPC.h"

#include "OInteractive.h"
#include "OStrategicPawn.h"

AOStrategicPC::AOStrategicPC()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AOStrategicPC::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AOStrategicPC::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AOStrategicPC::OnSelectObject);
	InputComponent->BindAxis("MoveForward", this, &AOStrategicPC::OnMoveForward);
	InputComponent->BindAxis("MoveRight", this, &AOStrategicPC::OnMoveRight);
}

void AOStrategicPC::OnMoveForward(const float Value)
{
	if (Value != 0.0f)
	{
		// TODO: Find if costly, find why not to just cache reference in BeginPlay
		AOStrategicPawn* StrategicCameraPlayer = Cast<AOStrategicPawn>(GetPawn());

		if (StrategicCameraPlayer != nullptr)
		{
			StrategicCameraPlayer->MoveForward(Value);
		}
	}
}

void AOStrategicPC::OnMoveRight(const float Value)
{
	if (Value != 0.0f)
	{
		// TODO: Find if costly, find why not to just cache reference in BeginPlay
		AOStrategicPawn* StrategicCameraPlayer = Cast<AOStrategicPawn>(GetPawn());

		if (StrategicCameraPlayer != nullptr)
		{
			StrategicCameraPlayer->MoveRight(Value);
		}
	}
}

void AOStrategicPC::OnSelectObject()
{
	// Trace to see what is under the touch location
	FHitResult Hit;
	GetHitResultUnderCursor(CurrentClickTraceChannel, true, Hit);

	if (Hit.bBlockingHit && Hit.GetComponent()->GetClass()->ImplementsInterface(UOInteractive::StaticClass()))
	{
		IOInteractive::Execute_OnInteractStart(Hit.GetComponent());
	}
}