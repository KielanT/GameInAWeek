// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerActor.generated.h"

UCLASS()
class GAMEINAWEEK_API ASpawnerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SpawnActor();

	FTransform SelectSpawnPoint();
	
private:
	UPROPERTY(EditAnywhere)
		USceneComponent* Root;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> Actor;

	UPROPERTY(EditAnywhere)
		USceneComponent* MinSpawn;
		
	UPROPERTY(EditAnywhere)
		USceneComponent* MaxSpawn;

	UPROPERTY(EditAnywhere)
		float ObjectSpeed = 300.0f;
	
	UPROPERTY()
		class AGameInAWeekGameMode* GameMode;

	UPROPERTY()
		float SpawnerTime = 0;

	UPROPERTY()
		float SpawnRate = 1.0f;

	UPROPERTY()
		float SpawnRateMin;

	UPROPERTY()
		float SpawnRateMax;

	UPROPERTY()
		float GameTimer = 0;

	UPROPERTY()
		float GameTimerRate;
};
