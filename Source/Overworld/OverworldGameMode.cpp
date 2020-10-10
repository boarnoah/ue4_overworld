// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverworldGameMode.h"
#include "OverworldPlayerController.h"
#include "OverworldCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOverworldGameMode::AOverworldGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AOverworldPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}