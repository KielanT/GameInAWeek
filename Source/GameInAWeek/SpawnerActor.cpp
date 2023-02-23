// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerActor.h"

#include "FallingActor.h"

// Sets default values
ASpawnerActor::ASpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnerActor::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ASpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Spawner += DeltaTime;
	if(Spawner >= MaxSpawnTime)
	{
		SpawnActor();
		Spawner = 0;
	}
}

void ASpawnerActor::SpawnActor()
{
	if(Actor)
	{
		FActorSpawnParameters params;
		AFallingActor* ActorRef = GetWorld()->SpawnActor<AFallingActor>(Actor, GetTransform(), params);
		ActorRef->SetSpeed(40.0f);
	}
}

