// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "OGameState.h"
#include "GameFramework/GameModeBase.h"
#include "OverworldGameMode.generated.h"

class AOTacticalCharacter;
class ULevelStreamingDynamic;
class AOStrategicCharacter;
class AOStrategicEncounter;
UCLASS(minimalapi)
class AOverworldGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AOverworldGameMode();

	UPROPERTY(Transient)
	AOGameState* OGameState;
	virtual void InitGameState() override;

	void OnEncounterStart(AOStrategicEncounter* Encounter, AOStrategicCharacter* Character);
	void OnEncounterEnd(AOTacticalCharacter* Character);

	// TODO: In reality this would be data assets containing lists of levels etc...
	UPROPERTY(EditDefaultsOnly)
	FName EncounterLevel;

	// Spawn tactical pawns, un-posses from strategic and posses the tactical pawns
	UFUNCTION()
	void SpawnAndTransferPlayersToTactical();
	// Repossess strategic pawns
	void TransferPlayersToStrategic();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	TSubclassOf<AOTacticalCharacter> TacticalCharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector EncounterLevelOffset = FVector(0, 0, 1000);
};



