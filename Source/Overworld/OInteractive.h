// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OInteractive.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOInteractive : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OVERWORLD_API IOInteractive
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteractStart();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteractStop();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnTandemInteractActor(AActor* Actor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void OnTandemInteractLocation(FVector Location);
};
