// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OStrategicPC.generated.h"

/**
 * 
 */
UCLASS()
class OVERWORLD_API AOStrategicPC : public APlayerController
{
public:
	AOStrategicPC();
	virtual void PlayerTick(float DeltaTime) override;
protected:
	virtual void SetupInputComponent() override;
private:
	GENERATED_BODY()

	void OnMoveForward(float Value);
	void OnMoveRight(float Value);
	void OnSelectObject();
};
