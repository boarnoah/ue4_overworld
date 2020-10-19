// Fill out your copyright notice in the Description page of Project Settings.


#include "OStrategicEncounter.h"


#include "OStrategicCharacter.h"
#include "OverworldGameMode.h"
#include "Components/SphereComponent.h"

// Sets default values
AOStrategicEncounter::AOStrategicEncounter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	StartZone = CreateDefaultSubobject<USphereComponent>(TEXT("StartZone"));
	StartZone->SetSphereRadius(100.0f);
	StartZone->SetupAttachment(GetRootComponent());

	StartZone->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StartZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	StartZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	StartZone->OnComponentBeginOverlap.AddDynamic(this, &AOStrategicEncounter::OnOverlap);
}

// Called when the game starts or when spawned
void AOStrategicEncounter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOStrategicEncounter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOStrategicEncounter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOStrategicCharacter* StrategicCharacter = Cast<AOStrategicCharacter>(OtherActor);

	if(StrategicCharacter)
	{
		AOverworldGameMode* Gm = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());

		if (Gm)
		{
			Gm->OnEncounterStart(this, StrategicCharacter);
		}
	}
}