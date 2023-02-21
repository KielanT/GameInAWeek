// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerActor.h"

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

	if(Actor)
	{
		FActorSpawnParameters params;
		AActor* ActorRef = GetWorld()->SpawnActor<AActor>(Actor, GetTransform(), params);
	}
}

// Called every frame
void ASpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

