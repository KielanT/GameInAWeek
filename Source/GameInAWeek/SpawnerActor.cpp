// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerActor.h"

#include "FallingActor.h"

// Sets default values
ASpawnerActor::ASpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	MaxSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("Max Spawn"));
	MaxSpawn->SetupAttachment(Root);
	
	MinSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("Min Spawn"));
	MinSpawn->SetupAttachment(Root);
	
}

// Called when the game starts or when spawned
void ASpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	SpawnRate = FMath::RandRange(0.1f, 1.0f);
	
	
}

// Called every frame
void ASpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SpawnerTime += DeltaTime;
	if(SpawnerTime >= SpawnRate)
	{
		SpawnActor();
		SpawnerTime = 0.0f; // Reset timer
		SpawnRate = FMath::RandRange(0.8f, 1.6f);
	}
}

void ASpawnerActor::SpawnActor()
{
	if(Actor)
	{
		FActorSpawnParameters params;
		AFallingActor* ActorRef = GetWorld()->SpawnActor<AFallingActor>(Actor, SelectSpawnPoint(), params);
		ActorRef->SetSpeed(400.0f);
	}
}

FTransform ASpawnerActor::SelectSpawnPoint()
{
	FTransform Transform = GetTransform();
	FVector NewLocation = Transform.GetLocation();
	NewLocation.Z = FMath::RandRange(MinSpawn->GetComponentLocation().Z, MaxSpawn->GetComponentLocation().Z);
	Transform.SetLocation(NewLocation);
	return Transform;
}

