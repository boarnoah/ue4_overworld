#pragma once
#include "OSaveState.generated.h"

// Extremely simplified save state from Estranged.Core (https://github.com/alanedwardes/Estranged.Core)
USTRUCT(BlueprintType)
struct FOActorState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<uint8> ActorData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SaveId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ActorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ActorTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* ActorClass;
};

USTRUCT(BlueprintType)
struct FOWorldState
{
	GENERATED_BODY()

	// For this PoC only care about storing strategic pawns & character's state
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FOActorState> ActorStates;
};