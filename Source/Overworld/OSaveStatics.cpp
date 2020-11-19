// Fill out your copyright notice in the Description page of Project Settings.


#include "OSaveStatics.h"
#include "EngineUtils.h"
#include "OSaveRestore.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

// This is extremely cut-down from the original Estranged Save (this only saves a handful - player actors for PoC)
FOWorldState UOSaveStatics::SerializeWorld(UObject* WorldContextObject)
{
	FOWorldState WorldState;

	UWorld* World = WorldContextObject->GetWorld();
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* actor = *It;
		if (actor->IsPendingKill())
		{
			continue;
		}

		if(actor->Implements<IOSaveRestore>())
		{
			UE_LOG(LogTemp, Log, TEXT("Serialized actor %s"), actor->GetFName());
			FOActorState ActorState;
            SerializeActor(actor, ActorState);
            WorldState.ActorStates.Add(ActorState);
		}
	}

	return WorldState;
}

void UOSaveStatics::SerializeActor(AActor* Actor, FOActorState& ActorState)
{
	ActorState.SaveId = IOSaveRestore::Execute_GetSaveId(Actor);
	ActorState.ActorName = Actor->GetFName();
	ActorState.ActorClass = Actor->GetClass();
	ActorState.ActorTransform = Actor->GetActorTransform();
	SerializeLowLevel(Actor, ActorState.ActorData);
}

void UOSaveStatics::RestoreWorld(UObject* WorldContextObject, FOWorldState WorldState)
{
	TArray<UObject*> RestoredObjects;

	UWorld* World = WorldContextObject->GetWorld();
	for(FOActorState ActorState : WorldState.ActorStates)
	{
		bool bFoundMatch = false;

		// Restore state on saved actors after matching to actor
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			AActor* actor = *It;
			if(actor->Implements<IOSaveRestore>())
			{
				bFoundMatch = IOSaveRestore::Execute_GetSaveId(actor) == ActorState.SaveId;
			
				if (bFoundMatch)
				{
					RestoreActor(actor, ActorState);
					RestoredObjects.Add(actor);
					break;
				}
			}
		}

		if (!bFoundMatch)
		{
			UE_LOG(LogTemp, Error, TEXT("Couldn't find match for saved actor %s!"), *ActorState.SaveId);
		}
	}
}

void UOSaveStatics::RestoreActor(AActor* Actor, const FOActorState& ActorState)
{
	Actor->SetActorTransform(ActorState.ActorTransform);
	RestoreLowLevel(Actor, ActorState.ActorData);
}

void UOSaveStatics::SerializeLowLevel(UObject* Object, TArray<uint8>& InBytes)
{
	FMemoryWriter MemoryWriter(InBytes, true);
	FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, true);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;
	Object->Serialize(Ar);
}

void UOSaveStatics::RestoreLowLevel(UObject* Object, TArray<uint8> Bytes)
{
	FMemoryReader MemoryReader(Bytes, true);
	FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
	Object->Serialize(Ar);
}
