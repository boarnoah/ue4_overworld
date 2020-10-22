// Fill out your copyright notice in the Description page of Project Settings.


#include "OGameState.h"


#include "OverworldGameMode.h"
#include "Engine/LevelStreamingDynamic.h"

void AOGameState::StreamLoadEncounterLevel_Implementation(FName Level, FVector EncounterLevelOffset)
{
	bool bSuccessfulLoad = false;
	StreamedEncounterLevel = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), Level.ToString(), EncounterLevelOffset, FRotator(0, 0, 0), bSuccessfulLoad);

	if (bSuccessfulLoad)
	{
		InEncounter = true;

		if (GetLocalRole() == ROLE_Authority )
		{
			AOverworldGameMode* gm = Cast<AOverworldGameMode>(AuthorityGameMode);
			StreamedEncounterLevel->OnLevelShown.AddDynamic(gm, &AOverworldGameMode::SpawnAndTransferPlayersToTactical);
		}
	}
}

void AOGameState::StreamUnloadEncounterLevel_Implementation(FName Level)
{
	if (GetLocalRole() == ROLE_Authority )
	{
		AOverworldGameMode* gm = Cast<AOverworldGameMode>(AuthorityGameMode);
		gm->TransferPlayersToStrategic();
	}

	InEncounter = false;
	StreamedEncounterLevel->SetShouldBeLoaded(false);
}
