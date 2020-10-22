// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverworldGameMode.h"


#include "EngineUtils.h"
#include "OStrategicCharacter.h"
#include "OStrategicEncounter.h"
#include "OStrategicPawn.h"
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

void AOverworldGameMode::InitGameState()
{
	Super::InitGameState();

	OGameState = Cast<AOGameState>(GameState);

	if (OGameState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode should be subclass of OGameState"));
	}
}

void AOverworldGameMode::OnEncounterStart(AOStrategicEncounter* Encounter, AOStrategicCharacter* Character)
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

	// Using a simplistic toggle to stream in and out level for testing

	if (!OGameState->InEncounter)
	{
		UE_LOG(LogTemp, Log, TEXT("Encounter %s Overlap with Character %s, starting encounter"), *Encounter->GetName(), *Character->GetName())
		OGameState->StreamLoadEncounterLevel(EncounterLevel, EncounterLevelOffset);
	}
}

void AOverworldGameMode::OnEncounterEnd(AOTacticalCharacter* Character)
{
	OGameState->InEncounter = false;
	OGameState->StreamUnloadEncounterLevel(EncounterLevel);
}

void AOverworldGameMode::SpawnAndTransferPlayersToTactical()
{
	// Don't want this re-trigger after initial posses (ex: when level is unloading)
	OGameState->StreamedEncounterLevel->OnLevelShown.RemoveDynamic(this, &AOverworldGameMode::SpawnAndTransferPlayersToTactical);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	ActorSpawnParams.Owner = this;

	// Can't use TActorIterator from world since we only want player start actors  from the encounter level
	TArray<AActor*> EncounterLevelActors = OGameState->StreamedEncounterLevel->GetLoadedLevel()->Actors;
	TArray<APlayerStart*> PlayerStarts;

	// Find player starts in the encounter level
	for (AActor* Actor : EncounterLevelActors)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);

		if (PlayerStart != nullptr)
		{
			PlayerStarts.Emplace(PlayerStart);
		}
	}

	if (GetNumPlayers() > PlayerStarts.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Not enough player start locations to spawn tactical pawns!"));
		return;
	}

	auto PlayerStartsItr = PlayerStarts.CreateConstIterator();

	// Spawn tactical pawns and make player controllers posses
	for (FConstPlayerControllerIterator PcItr = GetWorld()->GetPlayerControllerIterator(); PcItr; ++PcItr)
	{
		AOTacticalCharacter* TacticalActor =  GetWorld()->SpawnActor<AOTacticalCharacter>(TacticalCharacterClass, (*PlayerStartsItr)->GetActorLocation(), (*PlayerStartsItr)->GetActorRotation(), ActorSpawnParams);
		APawn* TacticalPawn = Cast<APawn>(TacticalActor);

		if(TacticalPawn != nullptr)
		{
			PcItr->Get()->Possess(TacticalPawn);
		}

		++PlayerStartsItr;
	}
}

void AOverworldGameMode::TransferPlayersToStrategic()
{
	FConstPlayerControllerIterator PcItr = GetWorld()->GetPlayerControllerIterator();
	const int NumControllers = GetWorld()->GetNumPlayerControllers();

	for (TActorIterator<AOStrategicPawn> It(GetWorld()); It; ++It)
	{
		if (PcItr.GetIndex() < NumControllers)
		{
            (*PcItr)->Possess(*It);
            ++PcItr;
			(*It)->OnRePossesByPlayer();
		} else
		{
			break;
		}
	}
}
