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
	if (!InEncounter)
	{
		UE_LOG(LogTemp, Log, TEXT("Encounter %s Overlap with Character %s, starting encounter"), *Encounter->GetName(), *Character->GetName())
		StreamLoadEncounterLevel(EncounterLevel);
	}
}

void AOverworldGameMode::OnEncounterEnd(AOTacticalCharacter* Character)
{
	InEncounter = false;
	StreamUnloadEncounterLevel(EncounterLevel);
}

void AOverworldGameMode::StreamLoadEncounterLevel(FName Level)
{
	bool bSuccessfulLoad = false;
	StreamedEncounterLevel = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), Level.ToString(), EncounterLevelOffset, FRotator(0, 0, 0), bSuccessfulLoad);

	if (bSuccessfulLoad)
	{
		InEncounter = true;
		StreamedEncounterLevel->OnLevelShown.AddDynamic(this, &AOverworldGameMode::SpawnAndTransferPlayersToTactical);
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
	// Don't want this re-trigger after initial posses (ex: when level is unloading)
	StreamedEncounterLevel->OnLevelShown.RemoveDynamic(this, &AOverworldGameMode::SpawnAndTransferPlayersToTactical);

	TArray<AActor*> EncounterLevelActors = StreamedEncounterLevel->GetLoadedLevel()->Actors;
	for (AActor* Actor : EncounterLevelActors)
	{
		if (Actor->IsA<APlayerStart>())
		{
			UE_LOG(LogTemp, Log, TEXT("Found spawn point"));

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.Owner = this;

			AOTacticalCharacter* TacticalActor =  GetWorld()->SpawnActor<AOTacticalCharacter>(TacticalCharacterClass, Actor->GetActorLocation(), Actor->GetActorRotation(), ActorSpawnParams);
			APawn* TacticalPawn = Cast<APawn>(TacticalActor);
			UE_LOG(LogTemp, Log, TEXT("Spawned tactical character"));

			// For simplicity for PoC just change possession for first player controller
			APlayerController* Pc =  GetWorld()->GetFirstPlayerController();

			if(TacticalPawn != nullptr && Pc != nullptr)
			{
				UE_LOG(LogTemp, Log, TEXT("Possessed tactical character"));
				Pc->Possess(TacticalPawn);
			}

			break;
		}
	}

}

void AOverworldGameMode::TransferPlayersToStrategic()
{
	APlayerController* Pc = GetWorld()->GetFirstPlayerController();

	for (TActorIterator<AOStrategicPawn> It(GetWorld()); It; ++It)
	{
		UE_LOG(LogTemp, Log, TEXT("Possessed strategic character"));
		Pc->Possess(*It);
		break;
	}
}
