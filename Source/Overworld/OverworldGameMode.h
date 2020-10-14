// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "OverworldGameMode.generated.h"

class AOStrategicCharacter;
class AOStrategicEncounter;
UCLASS(minimalapi)
class AOverworldGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOverworldGameMode();

	void OnEncounterOverlap(AOStrategicEncounter* Encounter, AOStrategicCharacter* Character);
};



