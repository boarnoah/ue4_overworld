// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OGameState.generated.h"

class ULevelStreamingDynamic;
/**
 * 
 */
UCLASS()
class OVERWORLD_API AOGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	// Approach of keeping persistent game-mode + level and streaming in the encounter maps
	UFUNCTION(NetMulticast, Reliable)
	void StreamLoadEncounterLevel(FName Level, FVector EncounterLevelOffset);
	UFUNCTION(NetMulticast, Reliable)
	void StreamUnloadEncounterLevel(FName Level);

	// Track whether in encounter or not
	UPROPERTY()
	bool InEncounter;

	// Reference to streamed encounter level, to unload
	UPROPERTY()
	ULevelStreamingDynamic* StreamedEncounterLevel;
};
