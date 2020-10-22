// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OStrategicPawn.generated.h"

UCLASS()
class OVERWORLD_API AOStrategicPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AOStrategicPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	UFUNCTION(Client, Reliable)
    void OnRePossesByPlayer();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* Sphere;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UFloatingPawnMovement* Movement;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* SelectedActor;

	void OnMoveForward(float Value);
	void OnMoveRight(float Value);
	void OnSelectObject();
	void OnSelectSecondary();
	FVector GetPointOnNavMesh(FVector Point) const;
	UFUNCTION(Server, Reliable, WithValidation)
	void OnServerSelectSecondary(AActor* Actor, FHitResult Hit);
};
