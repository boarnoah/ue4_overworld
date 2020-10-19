// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverworldGameMode.h"


#include "OStrategicCharacter.h"
#include "OStrategicEncounter.h"
#include "OTacticalCharacter.h"
#include "OverworldPlayerController.h"
#include "Engine/LevelStreamingDynamic.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
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

void AOverworldGameMode::OnEncounterOverlap(AOStrategicEncounter* Encounter, AOStrategicCharacter* Character)
{
	/* TODO: Implement and compare the different approaches
	 * 1. Stream in level out of view, possess character there, return once that encounter finished
	 * a. Encounter logic as part of game-mode
	 * b. Encounter logic in a structure similar to how Subsystems are implemented in UE4, then enabled or disabled
	 * depending on encounter type. Use GameState for module states
	 * 
	 * 2. Load to self contained game-mode for encounter, persist and restore over-world game-mode, state after finished
	 * a. Save over-world state to SaveObjects, Look into if in memory save is a possibility
	 * b. Override game-instance, switcharoo game-modes (i.e never throw away state). <- kind of pointless if actor
	 * state needs to be thrown away and rehydrated
	 */

	// TODO: Add return to strategic from tactical
	// Using a simplistic toggle to stream in and out level for testing
	if (!InEncounter)
	{
		UE_LOG(LogTemp, Log, TEXT("Encounter %s Overlap with Character %s, starting encounter"), *Encounter->GetName(), *Character->GetName())
		StreamLoadEncounterLevel(EncounterLevel);
	} else
	{
		StreamUnloadEncounterLevel(EncounterLevel);
	}
}

void AOverworldGameMode::StreamLoadEncounterLevel(FName Level)
{
	bool bSuccessfulLoad = false;
	StreamedEncounterLevel = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), Level.ToString(), EncounterLevelOffset, FRotator(0, 0, 0), bSuccessfulLoad);

	if (bSuccessfulLoad)
	{
		InEncounter = true;
		UE_LOG(LogTemp, Log, TEXT("Successfully loaded level, changing chars"));
	}
}

void AOverworldGameMode::StreamUnloadEncounterLevel(FName Level)
{
	InEncounter = false;
	StreamedEncounterLevel->SetShouldBeLoaded(false);
	TransferPlayersToStrategic();
}


void AOverworldGameMode::SpawnAndTransferPlayersToTactical()
{
	UE_LOG(LogTemp, Log, TEXT("Spawning and possessing tactical pawns"));
}

void AOverworldGameMode::TransferPlayersToStrategic()
{
	UE_LOG(LogTemp, Log, TEXT("Possessing stategic player pawn"));
}
