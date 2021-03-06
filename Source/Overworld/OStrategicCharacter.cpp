// Fill out your copyright notice in the Description page of Project Settings.


#include "OStrategicCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AOStrategicCharacter::AOStrategicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AOStrategicCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOStrategicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOStrategicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AOStrategicCharacter::OnTandemInteractLocation_Implementation(FVector Location)
{
	UE_LOG(LogTemp, Log, TEXT("Tandem interact order received"));
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Location);
}

void AOStrategicCharacter::OnTandemInteractActor_Implementation(AActor* Actor)
{
}

void AOStrategicCharacter::OnInteractStop_Implementation()
{
}

void AOStrategicCharacter::OnInteractStart_Implementation()
{
}

FString AOStrategicCharacter::GetSaveId_Implementation()
{
	return GetFName().ToString() + "_" + FGuid().ToString();
}

void AOStrategicCharacter::OnPostRestore_Implementation()
{
}