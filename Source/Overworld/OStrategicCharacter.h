// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "OInteractive.h"
#include "GameFramework/Character.h"
#include "OStrategicCharacter.generated.h"

UCLASS()
class OVERWORLD_API AOStrategicCharacter : public ACharacter, public IOInteractive
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOStrategicCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteractStart();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void OnInteractStop();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void OnTandemInteractActor(AActor* Actor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void OnTandemInteractLocation(FVector Location);
};
