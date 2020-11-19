// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "OSaveState.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OSaveStatics.generated.h"

// Taken from Save system in Estranged.Core (https://github.com/alanedwardes/Estranged.Core)
UCLASS()
class OVERWORLD_API UOSaveStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	// Handle serializing relevant properties
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = Saving)
	static FOWorldState SerializeWorld(UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category = Saving)
    static void SerializeActor(AActor* Actor, FOActorState& ActorState);

	// Handle hydrating new actors with the saved state data
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = Saving)
	static void RestoreWorld(UObject* WorldContextObject, FOWorldState WorldState);
	UFUNCTION(BlueprintCallable, Category = Saving)
    static void RestoreActor(AActor* Actor, const FOActorState &ActorState);

	// Call UE's serialize/deserialize to Archive things
	UFUNCTION(BlueprintCallable, Category = Saving)
    static void SerializeLowLevel(UObject* Object, TArray<uint8>& InBytes);
	UFUNCTION(BlueprintCallable, Category = Saving)
	static void RestoreLowLevel(UObject* Object, TArray<uint8> Bytes);
};
