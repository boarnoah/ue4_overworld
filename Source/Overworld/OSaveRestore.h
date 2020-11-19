// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OSaveRestore.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOSaveRestore : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

// This is extremely cut-down from the original Estranged.Core Save system (https://github.com/alanedwardes/Estranged.Core)
class OVERWORLD_API IOSaveRestore
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Called after the object's state is restored. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void OnPostRestore();

	/** Called to get the customised name of an actor/component */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    FString GetSaveId();
};
