// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerActor.h"

#include "FallingActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameInAWeekGameMode.h"

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
	GameMode = Cast<AGameInAWeekGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	SpawnRateMin = 0.8f;
	SpawnRateMax = 1.6f;
	GameTimerRate = 25;
}

// Called every frame
void ASpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(GameMode->IsPlaying())
	{
		GameTimer += DeltaTime;
		SpawnerTime += DeltaTime;
		if(SpawnerTime >= SpawnRate)
		{
			SpawnActor();
			SpawnerTime = 0.0f; // Reset timer
			SpawnRate = FMath::RandRange(SpawnRateMin, SpawnRateMax);
		}
		if(GameTimer >= GameTimerRate)
		{
			GameTimer = 0;
			if(GameTimerRate <= 3)
			{
				GameTimerRate = 3;
			}else
			{
				GameTimerRate -= 2;
			}
			
			if(SpawnRateMin >= 0.1f)
			{
				SpawnRateMin = SpawnRateMin - 0.1f;
			}else
			{
				SpawnRateMin = 0.1f;
			}
			
			if(SpawnRateMax >= 0.1f)
			{
				SpawnRateMax = SpawnRateMax - 0.1f;
			}else
			{
				SpawnRateMax = 0.1f;
			}
		}
	}
}

void ASpawnerActor::SpawnActor()
{
	if(Actor)
	{
		FActorSpawnParameters params;
		AFallingActor* ActorRef = GetWorld()->SpawnActor<AFallingActor>(Actor, SelectSpawnPoint(), params);
		ActorRef->SetSpeed(ObjectSpeed);
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

