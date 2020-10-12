// Fill out your copyright notice in the Description page of Project Settings.


#include "OStrategicPC.h"

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
	// Bind jump events
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
