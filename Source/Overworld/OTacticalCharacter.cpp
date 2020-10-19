// Fill out your copyright notice in the Description page of Project Settings.


#include "OTacticalCharacter.h"

// Sets default values
AOTacticalCharacter::AOTacticalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOTacticalCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOTacticalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOTacticalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* pc = Cast<APlayerController>(Controller);
	if(pc)
	{
		pc->bShowMouseCursor = false;
		pc->DefaultMouseCursor = EMouseCursor::Crosshairs;
	}

	PlayerInputComponent->BindAxis("MoveForward", this, &AOTacticalCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AOTacticalCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AOTacticalCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AOTacticalCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}