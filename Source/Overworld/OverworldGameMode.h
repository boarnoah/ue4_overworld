// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "OverworldGameMode.generated.h"

class ULevelStreamingDynamic;
class AOStrategicCharacter;
class AOStrategicEncounter;
UCLASS(minimalapi)
class AOverworldGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOverworldGameMode();

	void OnEncounterOverlap(AOStrategicEncounter* Encounter, AOStrategicCharacter* Character);

	// TODO: In reality this would be data assets containing lists of levels etc...
	UPROPERTY(EditDefaultsOnly)
	FName EncounterLevel;

	// Approach of keeping persistent game-mode + level and streaming in the encounter maps
	void StreamLoadEncounterLevel(FName Level);
	void StreamUnloadEncounterLevel(FName Level);

	// Reference to streamed encounter level, to unload
	UPROPERTY()
	ULevelStreamingDynamic* StreamedEncounterLevel;

	// Track whether in encounter or not
	UPROPERTY()
	bool InEncounter;

	// Spawn tactical pawns, un-posses from strategic and posses the tactical pawns
	void SpawnAndTransferPlayersToTactical();
	// Repossess strategic pawns
	void TransferPlayersToStrategic();
};



